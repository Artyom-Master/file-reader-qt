#include <QCoreApplication>
#include <QtTest>
#include "wordscounterworker.h"

class tst_wordscounterworker : public QObject
{
    Q_OBJECT

public:
    tst_wordscounterworker();
    ~tst_wordscounterworker();

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testSingleWord();
    void testMultipleWords();
    void testWordFrequencies();
    void testResetFunctionality();
    void testEmptyInputIgnored();
};

tst_wordscounterworker::tst_wordscounterworker() {}

tst_wordscounterworker::~tst_wordscounterworker() {}

void tst_wordscounterworker::initTestCase() {}

void tst_wordscounterworker::cleanupTestCase() {}

void tst_wordscounterworker::testSingleWord()
{
    WordsCounterWorker worker;
    QSignalSpy spy(&worker, &WordsCounterWorker::topFrequentWordsListRecalculated);

    worker.addWordToMap("apple");

    QVERIFY(spy.count() > 0);
    QList<QVariant> args = spy.takeLast();
    auto result = qvariant_cast<std::vector<std::pair<QString, int>>>(args.at(0));

    QCOMPARE(result.size(), 1);
    QCOMPARE(result[0].first, QString("apple"));
    QCOMPARE(result[0].second, 1);
}

void tst_wordscounterworker::testMultipleWords()
{
    WordsCounterWorker worker;
    QSignalSpy spy(&worker, &WordsCounterWorker::topFrequentWordsListRecalculated);

    worker.addWordToMap("banana");
    worker.addWordToMap("apple");
    worker.addWordToMap("banana");

    QVERIFY(spy.count() > 0);
    QList<QVariant> args = spy.takeLast();
    auto result = qvariant_cast<std::vector<std::pair<QString, int>>>(args.at(0));

    QCOMPARE(result.size(), 2);
    QCOMPARE(result.back().first, QString("banana"));
    QCOMPARE(result.back().second, 2);
}

void tst_wordscounterworker::testWordFrequencies()
{
    WordsCounterWorker worker;
    QSignalSpy spy(&worker, &WordsCounterWorker::topFrequentWordsListRecalculated);

    QStringList words = {"a", "b", "a", "c", "b", "a", "d", "e", "d", "d"};
    for (const QString& word : words)
        worker.addWordToMap(word);

    QVERIFY(spy.count() > 0);
    QList<QVariant> args = spy.takeLast();
    auto result = qvariant_cast<std::vector<std::pair<QString, int>>>(args.at(0));

    // Top 3 should be: a (3), d (3), b (2) — order not guaranteed between a and d
    QVERIFY(result.size() <= words.size());
    QList<QString> topWords;
    for (const auto& pair : result)
        topWords << pair.first;

    QVERIFY(topWords.contains("a"));
    QVERIFY(topWords.contains("d"));
    QVERIFY(topWords.contains("b"));
}

void tst_wordscounterworker::testResetFunctionality()
{
    WordsCounterWorker worker;
    QSignalSpy spy(&worker, &WordsCounterWorker::topFrequentWordsListRecalculated);

    worker.addWordToMap("reset");
    worker.resetCountedWordsMap();
    worker.addWordToMap("new");

    QVERIFY(spy.count() > 0);
    QList<QVariant> args = spy.takeLast();
    auto result = qvariant_cast<std::vector<std::pair<QString, int>>>(args.at(0));

    QCOMPARE(result.size(), 1);
    QCOMPARE(result[0].first, QString("new"));
    QCOMPARE(result[0].second, 1);
}

void tst_wordscounterworker::testEmptyInputIgnored()
{
    WordsCounterWorker worker;
    QSignalSpy spy(&worker, &WordsCounterWorker::topFrequentWordsListRecalculated);

    worker.addWordToMap("");
    worker.addWordToMap("valid");

    QVERIFY(spy.count() > 0);
    QList<QVariant> args = spy.takeLast();
    auto result = qvariant_cast<std::vector<std::pair<QString, int>>>(args.at(0));

    QCOMPARE(result.size(), 1);
    QCOMPARE(result[0].first, QString("valid"));
    QCOMPARE(result[0].second, 1);
}

QTEST_MAIN(tst_wordscounterworker)

#include "tst_wordscounterworker.moc"
