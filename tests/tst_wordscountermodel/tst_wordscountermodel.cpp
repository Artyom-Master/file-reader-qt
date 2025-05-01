#include <QCoreApplication>
#include <QtTest>
#include "wordscountermodel.h"

// add necessary includes here

class tst_wordscountermodel : public QObject
{
    Q_OBJECT

public:
    tst_wordscountermodel();
    ~tst_wordscountermodel();

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testInitialState();
    void testSetTopFrequentWordsList();
    void testResetModel();
    void testInvalidIndices();
    void testRoleNames();
    void testEmptyInput();
};

tst_wordscountermodel::tst_wordscountermodel() {}

tst_wordscountermodel::~tst_wordscountermodel() {}

void tst_wordscountermodel::initTestCase() {}

void tst_wordscountermodel::cleanupTestCase() {}

void tst_wordscountermodel::testInitialState()
{
    WordsCounterModel model;
    QCOMPARE(model.rowCount(), 0);
    QVERIFY(!model.data(model.index(0, 0), WordsCounterModel::WordRole).isValid());
}

void tst_wordscountermodel::testSetTopFrequentWordsList()
{
    WordsCounterModel model;
    std::vector<std::pair<QString, int>> words = {
        {"hello", 5},
        {"world", 3}
    };
    model.setTopFrequentWordsList(words, 5);
    QCOMPARE(model.rowCount(), static_cast<int>(words.size()));
    for (int i = 0; i < model.rowCount(); ++i) {
        QCOMPARE(model.data(model.index(i, 0), WordsCounterModel::WordRole).toString(), words[i].first);
        QCOMPARE(model.data(model.index(i, 0), WordsCounterModel::CountRole).toInt(), words[i].second);
    }
}

void tst_wordscountermodel::testResetModel()
{
    WordsCounterModel model;
    std::vector<std::pair<QString, int>> words = {
        {"hello", 5},
        {"world", 3}
    };
    model.setTopFrequentWordsList(words, 5);
    model.resetModel();
    QCOMPARE(model.rowCount(), 0);
    QVERIFY(!model.data(model.index(0, 0), WordsCounterModel::WordRole).isValid());
}

void tst_wordscountermodel::testInvalidIndices()
{
    WordsCounterModel model;
    std::vector<std::pair<QString, int>> words = {
        {"hello", 5}
    };
    model.setTopFrequentWordsList(words, 5);
    QVERIFY(!model.data(model.index(-1, 0), WordsCounterModel::WordRole).isValid());
    QVERIFY(!model.data(model.index(1, 0), WordsCounterModel::WordRole).isValid());
}

void tst_wordscountermodel::testRoleNames()
{
    WordsCounterModel model;
    QHash<int, QByteArray> roles = model.roleNames();
    QVERIFY(roles.contains(WordsCounterModel::WordRole));
    QVERIFY(roles.contains(WordsCounterModel::CountRole));
    QCOMPARE(roles.value(WordsCounterModel::WordRole), QByteArray("word"));
    QCOMPARE(roles.value(WordsCounterModel::CountRole), QByteArray("count"));
}

void tst_wordscountermodel::testEmptyInput()
{
    WordsCounterModel model;
    std::vector<std::pair<QString, int>> emptyWords;
    model.setTopFrequentWordsList(emptyWords, 0);
    QCOMPARE(model.rowCount(), 0);
    QVERIFY(!model.data(model.index(0, 0), WordsCounterModel::WordRole).isValid());
}

QTEST_MAIN(tst_wordscountermodel)

#include "tst_wordscountermodel.moc"
