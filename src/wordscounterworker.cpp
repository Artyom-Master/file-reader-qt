#include "wordscounterworker.h"
#include <queue>
#include <QDebug>

namespace
{
constexpr int COUNT_OF_MOST_FREQUENT_WORDS_IN_FILE{ 15 };
}

WordsCounterWorker::WordsCounterWorker(QObject *parent)
    : QObject{parent}
    , m_countedWordsMap{}
{

}

void WordsCounterWorker::updateTopFrequentWordsList(QStringList words)
{
    if(words.isEmpty())
    {
        qDebug() << "Got empty words list";
        return;
    }

    qDebug() << "Add words list to countedWordsMap";

    for(const auto& word : words)
    {
        ++m_countedWordsMap[word];
    }

    recalculateTopFrequentWordsList();
}

void WordsCounterWorker::resetCountedWordsMap()
{
    qInfo() << "Reset countedWordsMap";
    m_countedWordsMap.clear();
}

void WordsCounterWorker::recalculateTopFrequentWordsList()
{   
    // Min-heap to keep top n elements
    using Pair = std::pair<QString, int>;
    auto cmp = [](const Pair& a, const Pair& b) { return a.second > b.second; };
    std::priority_queue<Pair, std::vector<Pair>, decltype(cmp)> heap(cmp);

    qInfo() << "Start search of top n frequent words";

    for (const auto& [word, count] : m_countedWordsMap)
    {
        heap.emplace(word, count);

        if (heap.size() > COUNT_OF_MOST_FREQUENT_WORDS_IN_FILE)
        {
            heap.pop();
        }
    }

    qDebug() << "Convert top n frequent words priority_queue to vector";

    // Extract results
    std::vector<std::pair<QString, int>> result;
    int maxWordCount = 0;

    while (!heap.empty())
    {
        maxWordCount = std::max(maxWordCount, heap.top().second);
        result.emplace_back(heap.top().first, heap.top().second);
        heap.pop();
    }

    qInfo() << "Send fount top of words to model";
    emit topFrequentWordsListRecalculated(result, maxWordCount);
}
