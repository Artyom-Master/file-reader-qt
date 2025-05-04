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
    , m_countProgress{ 0 }
    , m_statusInfoText{"Open any text file."}

    , m_fileReaderWorkerThread{}
    , m_wordsCounterWorkerThread{}

    , m_fileReaderWorker{}
    , m_wordsCounterWorker{}

    , m_wordsCounterModel{ wordsCounterModel }
    , m_getReadWordsTimer{}

    , m_stateMachine{}
    , m_initState{}
    , m_readyToStartState{}
    , m_processingState{}
    , m_finishedState{}
    , m_pausedState{}
{   
    //Setup FileReaderWorker
    connect(this, &Controller::gotFileName, &m_fileReaderWorker, &FileReaderWorker::openFile);
    connect(&m_fileReaderWorker, &FileReaderWorker::fileOpened, this, &Controller::setReadyToStartState);
    connect(this, &Controller::startButtonClicked, &m_fileReaderWorker, &FileReaderWorker::start);
    connect(&m_fileReaderWorker, &FileReaderWorker::finished, this, &Controller::stopTakingReadWords);
    connect(this, &Controller::tookLastReadWords, &m_fileReaderWorker, &FileReaderWorker::clearData);

    m_fileReaderWorker.moveToThread(&m_fileReaderWorkerThread);
    m_fileReaderWorkerThread.start();

    //Setup WordsCounterWorker and WordsCounterModel
    connect(this, &Controller::gotPortionOfWordsInFile, &m_wordsCounterWorker, &WordsCounterWorker::updateDataAndStart);
    connect(&m_wordsCounterWorker, &WordsCounterWorker::updatedTopFrequentWordsList, this, &Controller::updateTopFrequentWordsHistogram);

    m_wordsCounterWorker.moveToThread(&m_wordsCounterWorkerThread);
    m_wordsCounterWorkerThread.start();

    //Setup m_getReadWordsTimer
    connect(&m_getReadWordsTimer, &QTimer::timeout, this, &Controller::passReadWordsToWordsCounterWorker);

    //Setup states
    m_initState.assignProperty(this, "canOpenFile", true);
    m_initState.assignProperty(this, "canStart", false);
    m_initState.assignProperty(this, "canPause", false);
    m_initState.assignProperty(this, "canCancel", false);
    m_initState.assignProperty(this, "pauseButtonText", "Pause");
    m_initState.assignProperty(this, "countProgress", 0);
    m_initState.assignProperty(this, "statusInfoText", "Open any text file.");

    m_readyToStartState.assignProperty(this, "canOpenFile", true);
    m_readyToStartState.assignProperty(this, "canStart", true);
    m_readyToStartState.assignProperty(this, "canPause", false);
    m_readyToStartState.assignProperty(this, "canCancel", false);
    m_readyToStartState.assignProperty(this, "pauseButtonText", "Pause");
    m_readyToStartState.assignProperty(this, "countProgress", 0);

    m_processingState.assignProperty(this, "canOpenFile", false);
    m_processingState.assignProperty(this, "canStart", false);
    m_processingState.assignProperty(this, "canPause", true);
    m_processingState.assignProperty(this, "canCancel", true);
    m_processingState.assignProperty(this, "pauseButtonText", "Pause");

    m_finishedState.assignProperty(this, "canOpenFile", true);
    m_finishedState.assignProperty(this, "canStart", false);
    m_finishedState.assignProperty(this, "canPause", false);
    m_finishedState.assignProperty(this, "canCancel", false);
    m_finishedState.assignProperty(this, "pauseButtonText", "Pause");
    m_finishedState.assignProperty(this, "statusInfoText", "Words counting finished. Open file to start new process.");

    m_pausedState.assignProperty(this, "canOpenFile", false);
    m_pausedState.assignProperty(this, "canStart", false);
    m_pausedState.assignProperty(this, "canPause", true);
    m_pausedState.assignProperty(this, "canCancel", true);
    m_pausedState.assignProperty(this, "pauseButtonText", "Continue");

    m_initState.addTransition(this, &Controller::readyToStart, &m_readyToStartState);
    m_readyToStartState.addTransition(this, &Controller::startButtonClicked, &m_processingState);
    m_processingState.addTransition(&m_wordsCounterWorker, &WordsCounterWorker::finished, &m_finishedState);
    m_finishedState.addTransition(&m_fileReaderWorker, &FileReaderWorker::fileOpened, &m_readyToStartState);
    m_processingState.addTransition(this, &Controller::pauseButtonClicked, &m_pausedState);
    m_pausedState.addTransition(this, &Controller::continueButtonClicked, &m_processingState);
    m_pausedState.addTransition(&m_wordsCounterWorker, &WordsCounterWorker::finished, &m_finishedState);

    m_stateMachine.addState(&m_initState);
    m_stateMachine.addState(&m_readyToStartState);
    m_stateMachine.addState(&m_processingState);
    m_stateMachine.addState(&m_finishedState);
    m_stateMachine.addState(&m_pausedState);
    m_stateMachine.setInitialState(&m_initState);
    m_stateMachine.start();
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
        emit gotFileName(fileUrl.toLocalFile());
    }
}

void Controller::setReadyToStartState(const QString& openedFileName)
{
    m_wordsCounterModel->clearData();
    setStatusInfoText(QString("File %1 is opened. You can start words counting.").arg(openedFileName));

    emit readyToStart();
}

void Controller::passReadWordsToWordsCounterWorker()
{
    auto readWords = m_fileReaderWorker.getReadWords();
    emit gotPortionOfWordsInFile(readWords.readWords, readWords.readingProgress);
}

void Controller::updateTopFrequentWordsHistogram(std::vector<std::pair<QString, int>> currentList, int maxWordCount, int readingProgress)
{
    setCountProgress(readingProgress);
    setStatusInfoText(QString("Progress of words counting: %1%.").arg(countProgress()));
    m_wordsCounterModel->setTopFrequentWordsList(currentList, maxWordCount);
}

void Controller::stopTakingReadWords()
{
    disableUIButtons();
    m_getReadWordsTimer.stop();

    auto readWords = m_fileReaderWorker.getReadWords();
    emit tookLastReadWords();

    emit gotPortionOfWordsInFile(readWords.readWords, readWords.readingProgress, true);
}

void Controller::startProcessing()
{
    qInfo() << "Start button clicked";

    setStatusInfoText(QString("Progress of words counting: %1%.").arg(0));
    m_getReadWordsTimer.start(INTERVAL_BETWEEN_HISTOGRAM_UPDATES_MS);

    emit startButtonClicked();
}

void Controller::pauseProcessing()
{
    if(pauseButtonText() == "Pause")
    {
        m_fileReaderWorker.pause();
        m_getReadWordsTimer.stop();
        m_wordsCounterWorker.pause();

        setStatusInfoText(QString("Words counting is paused(%1%). You can continue process or cancel it.").arg(countProgress()));
        emit pauseButtonClicked();
    }
    else
    {
        m_fileReaderWorker.resume();
        m_wordsCounterWorker.resume();
        m_getReadWordsTimer.start(INTERVAL_BETWEEN_HISTOGRAM_UPDATES_MS);

        setStatusInfoText(QString("Progress of words counting: %1%.").arg(countProgress()));
        emit continueButtonClicked();
    }
}

void Controller::cancelProcessing()
{
    m_fileReaderWorker.resume();
    m_fileReaderWorker.cancel();

    m_wordsCounterWorker.resume();
    m_wordsCounterWorker.cancel();

    m_wordsCounterModel->clearData();
    setCountProgress(0);
}

void Controller::disableUIButtons()
{
    setCanOpenFile(false);
    setCanStart(false);
    setCanPause(false);
    setCanCancel(false);
}
