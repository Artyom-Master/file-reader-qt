#ifndef FILEREADERWORKER_H
#define FILEREADERWORKER_H

#include <QObject>

class FileReaderWorker : public QObject
{
    Q_OBJECT
public:
    explicit FileReaderWorker(QObject *parent = nullptr);

signals:
};

#endif // FILEREADERWORKER_H
