#include <QCoreApplication>
#include <QtTest>
#include "wordscounterworker.h"

class tst_wordscounterworker : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase() {
        worker = new WordsCounterWorker;
    }
    void cleanupTestCase() {
        delete worker;
    }

    void testEmptyInputDoesNothing() {
        QSignalSpy spy(worker, &WordsCounterWorker::updatedTopFrequentWordsList);
        worker->updateDataAndStart({}, 10);
        QCOMPARE(spy.count(), 0);
    }

    void testUpdateDataAndStart_emitsCorrectly() {
        QSignalSpy spy(worker, &WordsCounterWorker::updatedTopFrequentWordsList);
        // Provide some data
        worker->updateDataAndStart({"a","b","a","c","b","a"}, 30);
        // run() is called inside updateDataAndStart
        QVERIFY(spy.count() == 1);
        auto args = spy.takeFirst();
        auto list = args.at(0).value<std::vector<std::pair<QString,int>>>();
        int maxCount = args.at(1).toInt();
        int progress = args.at(2).toInt();

        // Should find top 3 words: a(3), b(2), c(1) in any order
        QVERIFY(maxCount == 3);
        QCOMPARE(progress, 30);
        QMap<QString,int> map;
        for (auto &p : list) map[p.first] = p.second;
        QCOMPARE(map["a"], 3);
        QCOMPARE(map["b"], 2);
        QCOMPARE(map["c"], 1);
    }

    void testFinishWork() {
        QSignalSpy spy(worker, &WordsCounterWorker::finished);
        worker->updateDataAndStart({"x"}, 10);
        worker->finishWork();
        QCOMPARE(spy.count(), 1);
    }

private:
    WordsCounterWorker *worker;
};

QTEST_MAIN(tst_wordscounterworker)

#include "tst_wordscounterworker.moc"
