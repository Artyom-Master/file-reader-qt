#ifndef WORDSCOUNTERWORKER_H
#define WORDSCOUNTERWORKER_H

#include <QObject>

class WordsCounterWorker : public QObject
{
    Q_OBJECT
public:
    explicit WordsCounterWorker(QObject *parent = nullptr);

public slots:
    void updateTopFrequentWordsList(QStringList words);
    void resetCountedWordsMap();
    void recalculateTopFrequentWordsList();

signals:
    void topFrequentWordsListRecalculated(std::vector<std::pair<QString, int>> currentList, int maxWordCount);

private:
    std::unordered_map<QString, int> m_countedWordsMap;
};

#endif // WORDSCOUNTERWORKER_H
