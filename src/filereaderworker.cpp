#include "filereaderworker.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QFileInfo>
#include <QRegularExpression>

#include <mutex>

FileReaderWorker::FileReaderWorker(QObject *parent)
    : QObject{parent}
    , AbstractWorker{}
    , m_currentFile{}
    , m_readWords{}
    , m_readWordsMutex{}
{

}

FileReadingProgress FileReaderWorker::getReadWords()
{
    std::shared_lock lock(m_readWordsMutex);
    auto readWords = m_readWords;
    clearData();

    double readingProgress{ m_currentFile.pos() / static_cast<double>(m_currentFile.size()) };
    qDebug() << QString("Current progress: %1 / %2 = %3").arg(m_currentFile.pos()).arg(m_currentFile.size()).arg(readingProgress * 100);

    return {
            .readWords = readWords
            , .readingProgress = m_currentFile.isOpen() ? static_cast<int>(readingProgress * 100) : 100
    };
}

void FileReaderWorker::openFile(const QString &filePath)
{
    if(m_currentFile.isOpen())
    {
        qDebug() << QString("File %1 is closed before opening new file").arg(m_currentFile.fileName());
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
    emit fileOpened(QFileInfo(m_currentFile).fileName());
}

void FileReaderWorker::start()
{
    if(!m_currentFile.isOpen())
    {
        qWarning() << "File is not opened, can't start reading";

        emit errorOccurred("File is not opened, nothing to read");
        return;
    }

    qInfo() << QString("Start reading file %1").arg(m_currentFile.fileName());
    run();
    qInfo() << QString("Finish reading file %1").arg(m_currentFile.fileName());

    m_currentFile.close();
    emit finished();
}

void FileReaderWorker::run()
{
    QTextStream inStream(&m_currentFile);
    QString substring;

    while (!m_canceled && !inStream.atEnd())
    {
        inStream >> substring;
        if (!substring.isEmpty())
        {
            std::unique_lock lock(m_readWordsMutex);
            m_readWords.append(extractWords(substring));
        }

        callCVWait();
    }

    if(m_canceled)
    {
        m_canceled = false;
        clearData();
    }
}

void FileReaderWorker::clearData()
{
    m_readWords.clear();
}

QStringList FileReaderWorker::extractWords(const QString& input) {
    QRegularExpression regex("\\p{L}+");
    QRegularExpressionMatchIterator it = regex.globalMatch(input);
    QStringList words;

    while (it.hasNext()) {
        QRegularExpressionMatch match = it.next();
        words << match.captured(0);
    }

    return words;
}
