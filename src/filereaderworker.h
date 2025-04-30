#ifndef FILEREADERWORKER_H
#define FILEREADERWORKER_H

#include <QObject>
#include <QFile>

class FileReaderWorker : public QObject
{
    Q_OBJECT

public:
    explicit FileReaderWorker(QObject *parent = nullptr);

public slots:
    void openFile(const QString &filePath);
    void readFile();

signals:
    void fileOpened();
    void wordRead(QString word);
    void finished();
    void errorOccurred(const QString &error);

private:
    QFile m_currentFile;
};

#endif // FILEREADERWORKER_H
