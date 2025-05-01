#ifndef FILEREADERWORKER_H
#define FILEREADERWORKER_H

#include <QObject>
#include <QFile>

#include <shared_mutex>
#include <atomic>

class FileReaderWorker : public QObject
{
    Q_OBJECT

public:
    explicit FileReaderWorker(QObject *parent = nullptr);

    QStringList getReadWords();

public slots:
    void openFile(const QString &filePath);
    void readFile();
    void cancelFileReading();

signals:
    void fileOpened();
    void finished();
    void errorOccurred(const QString &error);

private:
    QFile m_currentFile;
    QStringList m_readWords;

    std::shared_mutex m_readWordsMutex;
    std::atomic<bool> m_cancelFileReading;
};

#endif // FILEREADERWORKER_H
