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
    , m_currentReadingProgress{ 0 }
{

}

void WordsCounterWorker::updateDataAndStart(QStringList words, int readingProgress)
{
    if(readingProgress <= 100 && readingProgress >= 0 && readingProgress >= m_currentReadingProgress)
    {
        m_currentReadingProgress = readingProgress;
    }

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

    qInfo() << "Start search of top n frequent words";
    run();
    qInfo() << "Finish search of top n frequent words";
}

void WordsCounterWorker::finishWork()
{
    clearData();
    emit finished();
}

void WordsCounterWorker::run()
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

        if(m_canceled)
        {
            m_canceled = false;
            return;
        }

        callCVWait();
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

        if(m_canceled)
        {
            m_canceled = false;
            return;
        }

        callCVWait();
    }

    qDebug() << "Send found top of words to model";
    emit updatedTopFrequentWordsList(result, maxWordCount, m_currentReadingProgress);
}

void WordsCounterWorker::clearData()
{
    qDebug() << "Reset countedWordsMap and reading progress";
    m_currentReadingProgress = 0;
    m_countedWordsMap.clear();
}
