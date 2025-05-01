#include "controller.h"

Controller::Controller(const std::shared_ptr<WordsCounterModel>& wordsCounterModel, QObject *parent)
    : QObject{parent}
    , m_canPause{ false }
    , m_canCancel{ false }

    , m_fileReaderWorkerThread{}
    , m_wordsCounterWorkerThread{}

    , m_fileReaderWorker{}
    , m_wordsCounterWorker{}

    , m_wordsCounterModel{ wordsCounterModel }
{
    connect(this, &Controller::openFileSignal, &m_fileReaderWorker, &FileReaderWorker::openFile);
    connect(this, &Controller::readFileSignal, &m_fileReaderWorker, &FileReaderWorker::readFile);

    m_fileReaderWorker.moveToThread(&m_fileReaderWorkerThread);
    m_fileReaderWorkerThread.start();

    connect(&m_fileReaderWorker, &FileReaderWorker::wordRead, &m_wordsCounterWorker, &WordsCounterWorker::addWordToMap);
    connect(&m_fileReaderWorker, &FileReaderWorker::finished, &m_wordsCounterWorker, &WordsCounterWorker::recalculateTopFrequentWordsList);
    connect(&m_wordsCounterWorker, &WordsCounterWorker::topFrequentWordsListRecalculated, m_wordsCounterModel.get(), &WordsCounterModel::setTopFrequentWordsList);

    m_wordsCounterWorker.moveToThread(&m_wordsCounterWorkerThread);
    m_wordsCounterWorkerThread.start();
}

Controller::~Controller()
{
    m_wordsCounterWorkerThread.quit();
    m_wordsCounterWorkerThread.wait(500);

    m_fileReaderWorkerThread.quit();
    m_fileReaderWorkerThread.wait(500);
}

void Controller::openFile(const QUrl& fileUrl)
{
    qInfo() << "Open button clicked";

    if(!fileUrl.isEmpty())
    {
        qDebug() << QString("File url is sent to FileReaderWorker: %1").arg(fileUrl.toLocalFile());
        emit openFileSignal(fileUrl.toLocalFile());
    }
}

void Controller::startProcessing()
{
    qInfo() << "Start button clicked";

    emit readFileSignal();
}

void Controller::pauseProcessing()
{

}

void Controller::cancelProcessing()
{

}
