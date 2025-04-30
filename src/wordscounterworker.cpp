#include "wordscounterworker.h"
#include <queue>

namespace
{
constexpr int COUNT_OF_MOST_FREQUENT_WORDS_IN_FILE{ 15 };
}

WordsCounterWorker::WordsCounterWorker(QObject *parent)
    : QObject{parent}
    , m_countedWordsMap{}
{

}

void WordsCounterWorker::addWordToMap(QString word)
{
    if(word.isEmpty())
    {
        return;
    }

    ++m_countedWordsMap[word];
    recalculateTopFrequentWordsList();
}

void WordsCounterWorker::resetCountedWordsMap()
{
    m_countedWordsMap.clear();
}

void WordsCounterWorker::recalculateTopFrequentWordsList()
{
    // Min-heap to keep top n elements
    using Pair = std::pair<QString, int>;
    auto cmp = [](const Pair& a, const Pair& b) { return a.second > b.second; };
    std::priority_queue<Pair, std::vector<Pair>, decltype(cmp)> heap(cmp);

    for (const auto& [word, count] : m_countedWordsMap)
    {
        heap.emplace(word, count);

        if (heap.size() > COUNT_OF_MOST_FREQUENT_WORDS_IN_FILE)
        {
            heap.pop();
        }
    }

    // Extract results
    std::vector<std::pair<QString, int>> result;

    while (!heap.empty())
    {
        result.emplace_back(heap.top().first, heap.top().second);
        heap.pop();
    }

    emit topFrequentWordsListRecalculated(result);
}
