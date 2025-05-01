#ifndef FILEREADERWORKER_H
#define FILEREADERWORKER_H

#include <QObject>
#include <QFile>

#include <shared_mutex>

#include "abstractworker/abstractworker.h"

class FileReaderWorker : public QObject, public AbstractWorker
{
    Q_OBJECT

public:
    explicit FileReaderWorker(QObject *parent = nullptr);

    QStringList getReadWords();

public slots:
    void openFile(const QString &filePath);
    void start();

signals:
    void fileOpened();
    void finished();
    void errorOccurred(const QString &error);

private:
    void run() override;

    QFile m_currentFile;
    QStringList m_readWords;

    std::shared_mutex m_readWordsMutex;
};

#endif // FILEREADERWORKER_H
