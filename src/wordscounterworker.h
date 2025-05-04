#ifndef WORDSCOUNTERWORKER_H
#define WORDSCOUNTERWORKER_H

#include <QObject>

#include "abstractworker/abstractworker.h"

class WordsCounterWorker : public QObject, public AbstractWorker
{
    Q_OBJECT
public:
    explicit WordsCounterWorker(QObject *parent = nullptr);

public slots:
    void updateDataAndStart(QStringList words, int readingProgress, bool last);

signals:
    void updatedTopFrequentWordsList(std::vector<std::pair<QString, int>> currentList, int maxWordCount, int readingProgress);
    void finished();

private:
    void run() override;
    void clearData();

    std::unordered_map<QString, int> m_countedWordsMap;
    int m_currentReadingProgress;
};

#endif // WORDSCOUNTERWORKER_H
