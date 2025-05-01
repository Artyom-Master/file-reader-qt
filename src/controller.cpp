#include "controller.h"

namespace
{
constexpr int INTERVAL_BETWEEN_HISTOGRAM_UPDATES_MS{ 33 };
}

Controller::Controller(const std::shared_ptr<WordsCounterModel>& wordsCounterModel, QObject *parent)
    : QObject{parent}
    , m_canPause{ false }
    , m_canCancel{ false }

    , m_fileReaderWorkerThread{}
    , m_wordsCounterWorkerThread{}

    , m_fileReaderWorker{}
    , m_wordsCounterWorker{}

    , m_wordsCounterModel{ wordsCounterModel }
    , m_getReadWordsTimer{}
{
    //Setup FileReaderWorker
    connect(this, &Controller::openFileSignal, &m_fileReaderWorker, &FileReaderWorker::openFile);
    connect(this, &Controller::readFileSignal, &m_fileReaderWorker, &FileReaderWorker::readFile);
    connect(&m_fileReaderWorker, &FileReaderWorker::finished, this, &Controller::finishProcessing);

    m_fileReaderWorker.moveToThread(&m_fileReaderWorkerThread);
    m_fileReaderWorkerThread.start();

    //Setup WordsCounterWorker
    connect(this, &Controller::startCountOfReadWords, &m_wordsCounterWorker, &WordsCounterWorker::updateTopFrequentWordsList);
    connect(&m_wordsCounterWorker, &WordsCounterWorker::topFrequentWordsListRecalculated, m_wordsCounterModel.get(), &WordsCounterModel::setTopFrequentWordsList);

    m_wordsCounterWorker.moveToThread(&m_wordsCounterWorkerThread);
    m_wordsCounterWorkerThread.start();

    //Setup m_getReadWordsTimer
    connect(&m_getReadWordsTimer, &QTimer::timeout, this, &Controller::updateTopFrequentWordsHistogram);
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

    m_getReadWordsTimer.start(INTERVAL_BETWEEN_HISTOGRAM_UPDATES_MS);
    emit readFileSignal();
}

void Controller::pauseProcessing()
{

}

void Controller::cancelProcessing()
{

}

void Controller::updateTopFrequentWordsHistogram()
{
    auto readWords = m_fileReaderWorker.getReadWords();
    emit startCountOfReadWords(readWords);
}

void Controller::finishProcessing()
{
    m_getReadWordsTimer.stop();
    updateTopFrequentWordsHistogram();
}
