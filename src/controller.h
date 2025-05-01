#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QThread>
#include <QUrl>
#include <QTimer>

#include "filereaderworker.h"
#include "wordscountermodel.h"
#include "wordscounterworker.h"

class Controller : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool canOpenFile READ canOpenFile WRITE setCanOpenFile NOTIFY canOpenFileChanged)
    Q_PROPERTY(bool canStart READ canStart WRITE setCanStart NOTIFY canStartChanged)
    Q_PROPERTY(bool canPause READ canPause WRITE setCanPause NOTIFY canPauseChanged)
    Q_PROPERTY(bool canCancel READ canCancel WRITE setCanCancel NOTIFY canCancelChanged)

public:
    explicit Controller(const std::shared_ptr<WordsCounterModel>& wordsCounterModel, QObject *parent = nullptr);
    ~Controller();

    bool canOpenFile() const { return m_canOpenFile; }
    void setCanOpenFile(bool canOpenFile) {
        if (m_canOpenFile != canOpenFile) {
            m_canOpenFile = canOpenFile;
            emit canOpenFileChanged();
        }
    }

    bool canStart() const { return m_canStart; }
    void setCanStart(bool canStart) {
        if (m_canStart != canStart) {
            m_canStart = canStart;
            emit canStartChanged();
        }
    }

    bool canPause() const { return m_canPause; }
    void setCanPause(bool canPause) {
        if (m_canPause != canPause) {
            m_canPause = canPause;
            emit canPauseChanged();
        }
    }

    bool canCancel() const { return m_canCancel; }
    void setCanCancel(bool canCancel) {
        if (m_canCancel != canCancel) {
            m_canCancel = canCancel;
            emit canCancelChanged();
        }
    }

public slots:
    void openFile(const QUrl& fileUrl);
    void startProcessing();
    void pauseProcessing();
    void cancelProcessing();

    void updateTopFrequentWordsHistogram();
    void finishProcessing();


signals:
    void canOpenFileChanged();
    void canStartChanged();
    void canPauseChanged();
    void canCancelChanged();

    void openFileSignal(const QString& fileUrl);
    void readFileSignal();

    void startCountOfReadWords(QStringList readWords);

private:
    bool m_canOpenFile;
    bool m_canStart;
    bool m_canPause;
    bool m_canCancel;

    QThread m_fileReaderWorkerThread;
    QThread m_wordsCounterWorkerThread;

    FileReaderWorker m_fileReaderWorker;
    WordsCounterWorker m_wordsCounterWorker;

    std::shared_ptr<WordsCounterModel> m_wordsCounterModel;

    QTimer m_getReadWordsTimer;
};

#endif // CONTROLLER_H
