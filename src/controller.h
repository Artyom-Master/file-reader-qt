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

    Q_PROPERTY(bool canPause READ canPause WRITE setCanPause NOTIFY canPauseChanged)
    Q_PROPERTY(bool canCancel READ canCancel WRITE setCanCancel NOTIFY canCancelChanged)

public:
    explicit Controller(const std::shared_ptr<WordsCounterModel>& wordsCounterModel, QObject *parent = nullptr);
    ~Controller();

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

    void finishProcessing();


signals:
    void canPauseChanged();
    void canCancelChanged();

    void openFileSignal(const QString& fileUrl);
    void readFileSignal();

    void startCountOfReadWords(QStringList readWords);

private:
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
