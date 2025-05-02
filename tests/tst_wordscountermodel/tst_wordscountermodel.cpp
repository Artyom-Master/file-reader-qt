#include <QCoreApplication>
#include <QtTest>
#include "wordscountermodel.h"

// add necessary includes here

class tst_wordscountermodel : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase() {
        model = new WordsCounterModel;
    }
    void cleanupTestCase() {
        delete model;
    }

    void testInitialState() {
        QCOMPARE(model->rowCount(), 0);                                                   // empty at start
        QVERIFY(!model->data(model->index(0,0), WordsCounterModel::WordRole).isValid()); // no data
    }

    void testRoleNames() {
        auto roles = model->roleNames();
        QVERIFY(roles.contains(WordsCounterModel::WordRole));
        QVERIFY(roles.contains(WordsCounterModel::CountRole));
        QVERIFY(roles.contains(WordsCounterModel::MaxWordCountRole));
    }

    void testSetTopFrequentWordsList() {
        std::vector<std::pair<QString,int>> entries = {
            { "apple", 5 }, { "banana", 3 }, { "cherry", 7 }
        };
        QSignalSpy lengthSpy(model, &WordsCounterModel::lengthChanged);
        QSignalSpy progressSpy(model, &WordsCounterModel::countProgressChanged);
        QSignalSpy statusSpy(model, &WordsCounterModel::statusInfoTextChanged);

        model->setTopFrequentWordsList(entries, /*max*/7, /*progress*/42);

        QCOMPARE(model->rowCount(), 3);
        // check first entry
        QCOMPARE(model->data(model->index(0,0), WordsCounterModel::WordRole).toString(), QString("apple"));
        QCOMPARE(model->data(model->index(0,0), WordsCounterModel::CountRole).toInt(), 5);
        QCOMPARE(model->data(model->index(0,0), WordsCounterModel::MaxWordCountRole).toInt(), 7);

        QCOMPARE(lengthSpy.count(), 1);
        QCOMPARE(progressSpy.count(), 1);
        QCOMPARE(statusSpy.count(), 1);
    }

    void testClearData() {
        model->clearData();
        QCOMPARE(model->rowCount(), 0);
        QCOMPARE(model->length(), 0);
        QCOMPARE(model->countProgress(), 0);
    }

private:
    WordsCounterModel *model;
};

QTEST_MAIN(tst_wordscountermodel)

#include "tst_wordscountermodel.moc"
