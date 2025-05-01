#include "controller.h"

namespace
{
constexpr int INTERVAL_BETWEEN_HISTOGRAM_UPDATES_MS{ 50 };
}

Controller::Controller(const std::shared_ptr<WordsCounterModel>& wordsCounterModel, QObject *parent)
    : QObject{parent}
    , m_canOpenFile{ true }
    , m_canStart{ false }
    , m_canPause{ false }
    , m_canCancel{ false }
    , m_pauseButtonText{ "Pause" }

    , m_fileReaderWorkerThread{}
    , m_wordsCounterWorkerThread{}

    , m_fileReaderWorker{}
    , m_wordsCounterWorker{}

    , m_wordsCounterModel{ wordsCounterModel }
    , m_getReadWordsTimer{}
{   
    //Setup FileReaderWorker
    connect(this, &Controller::openFileSignal, &m_fileReaderWorker, &FileReaderWorker::openFile);
    connect(this, &Controller::readFileSignal, &m_fileReaderWorker, &FileReaderWorker::start);
    connect(&m_fileReaderWorker, &FileReaderWorker::fileOpened, this, [this](const QString& openedFileName){
        emit clearHistogramData();
        m_wordsCounterModel->setStatusInfoText(QString("File %1 is opened. You can start words counting.").arg(openedFileName));
        setCanStart(true);
    });
    connect(&m_fileReaderWorker, &FileReaderWorker::finished, this, [this](){
        m_getReadWordsTimer.stop();
        updateTopFrequentWordsHistogram();
        emit finishWordsCounterWorker();
    });

    m_fileReaderWorker.moveToThread(&m_fileReaderWorkerThread);
    m_fileReaderWorkerThread.start();

    //Setup WordsCounterWorker and WordsCounterModel
    connect(this, &Controller::startCountOfReadWords, &m_wordsCounterWorker, &WordsCounterWorker::updateDataAndStart);
    connect(this, &Controller::finishWordsCounterWorker, &m_wordsCounterWorker, &WordsCounterWorker::finishWork);
    connect(&m_wordsCounterWorker, &WordsCounterWorker::updatedTopFrequentWordsList, m_wordsCounterModel.get(), &WordsCounterModel::setTopFrequentWordsList);
    connect(&m_wordsCounterWorker, &WordsCounterWorker::finished, this, &Controller::finishProcessing);
    connect(this, &Controller::clearHistogramData, m_wordsCounterModel.get(), &WordsCounterModel::clearData);

    m_wordsCounterWorker.moveToThread(&m_wordsCounterWorkerThread);
    m_wordsCounterWorkerThread.start();

    //Setup m_getReadWordsTimer
    connect(&m_getReadWordsTimer, &QTimer::timeout, this, &Controller::updateTopFrequentWordsHistogram);
}

Controller::~Controller()
{
    cancelProcessing();

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

void Controller::updateTopFrequentWordsHistogram()
{
    auto readWords = m_fileReaderWorker.getReadWords();
    emit startCountOfReadWords(readWords.readWords, readWords.readingProgress);
}

void Controller::startProcessing()
{
    qInfo() << "Start button clicked";

    m_getReadWordsTimer.start(INTERVAL_BETWEEN_HISTOGRAM_UPDATES_MS);
    emit readFileSignal();

    m_wordsCounterModel->setStatusInfoText(QString("Progress of words counting: %1%.").arg(0));
    setCanOpenFile(false);
    setCanStart(false);
    setCanPause(true);
    setCanCancel(true);
}

void Controller::pauseProcessing()
{
    if(m_pauseButtonText == "Pause")
    {
        m_fileReaderWorker.pause();
        m_getReadWordsTimer.stop();
        m_wordsCounterWorker.pause();

        setPauseButtonText("Continue");
        m_wordsCounterModel->setStatusInfoText(QString("Words counting is paused(%1%). You can continue process or cancel it.").arg(m_wordsCounterModel->countProgress()));
    }
    else
    {
        m_fileReaderWorker.resume();
        m_wordsCounterWorker.resume();
        m_getReadWordsTimer.start(INTERVAL_BETWEEN_HISTOGRAM_UPDATES_MS);

        setPauseButtonText("Pause");
    }
}

void Controller::cancelProcessing()
{
    m_getReadWordsTimer.stop();

    m_fileReaderWorker.resume();
    m_fileReaderWorker.cancel();

    m_wordsCounterWorker.resume();
    m_wordsCounterWorker.cancel();

    emit clearHistogramData();
}

void Controller::finishProcessing()
{
    m_wordsCounterModel->setStatusInfoText("Words counting finished. Open file to start new process.");
    setCanOpenFile(true);
    setCanStart(false);
    setCanPause(false);
    setCanCancel(false);
}
