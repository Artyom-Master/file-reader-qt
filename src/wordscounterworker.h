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
    void updateDataAndStart(QStringList words);
    void finishWork();

signals:
    void finished();
    void updatedTopFrequentWordsList(std::vector<std::pair<QString, int>> currentList, int maxWordCount);

private:
    void run() override;
    void clearData();

    std::unordered_map<QString, int> m_countedWordsMap;
};

#endif // WORDSCOUNTERWORKER_H
