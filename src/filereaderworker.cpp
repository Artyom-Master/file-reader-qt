#include "filereaderworker.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

FileReaderWorker::FileReaderWorker(QObject *parent)
    : QObject{parent}
    , m_currentFile{}
{

}

void FileReaderWorker::openFile(const QString &filePath)
{
    if(m_currentFile.isOpen())
    {
        m_currentFile.close();
    }

    m_currentFile.setFileName(filePath);

    if (!m_currentFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << QString("Failed to open file: %1").arg(m_currentFile.errorString());
        emit errorOccurred(QString("Failed to open file: %1").arg(m_currentFile.errorString()));
        return;
    }

    qInfo() << QString("Opened file %1").arg(filePath);
    emit fileOpened();
}

void FileReaderWorker::readFile()
{
    QTextStream inStream(&m_currentFile);
    QString word;

    while (!inStream.atEnd())
    {
        inStream >> word;
        if (!word.isEmpty())
        {
            emit wordRead(word);
        }
    }

    m_currentFile.close();
    emit finished();
}
