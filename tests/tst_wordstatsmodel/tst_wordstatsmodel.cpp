#include <QCoreApplication>
#include <QtTest>
#include "wordscountermodel.h"

// add necessary includes here

class tst_wordstatsmodel : public QObject
{
    Q_OBJECT

public:
    tst_wordstatsmodel();
    ~tst_wordstatsmodel();

private:
    WordsCounterModel m_wordStatsModel;

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

tst_wordstatsmodel::tst_wordstatsmodel() {}

tst_wordstatsmodel::~tst_wordstatsmodel() {}

void tst_wordstatsmodel::initTestCase() {}

void tst_wordstatsmodel::cleanupTestCase() {}

void tst_wordstatsmodel::testInitialState()
{
    WordsCounterModel model;
    QCOMPARE(model.rowCount(), 0);
    QVERIFY(!model.data(model.index(0, 0), WordsCounterModel::WordRole).isValid());
}

void tst_wordstatsmodel::testSetTopFrequentWordsList()
{
    WordsCounterModel model;
    std::vector<std::pair<QString, int>> words = {
        {"hello", 5},
        {"world", 3}
    };
    model.setTopFrequentWordsList(words);
    QCOMPARE(model.rowCount(), static_cast<int>(words.size()));
    for (int i = 0; i < model.rowCount(); ++i) {
        QCOMPARE(model.data(model.index(i, 0), WordsCounterModel::WordRole).toString(), words[i].first);
        QCOMPARE(model.data(model.index(i, 0), WordsCounterModel::CountRole).toInt(), words[i].second);
    }
}

void tst_wordstatsmodel::testResetModel()
{
    WordsCounterModel model;
    std::vector<std::pair<QString, int>> words = {
        {"hello", 5},
        {"world", 3}
    };
    model.setTopFrequentWordsList(words);
    model.resetModel();
    QCOMPARE(model.rowCount(), 0);
    QVERIFY(!model.data(model.index(0, 0), WordsCounterModel::WordRole).isValid());
}

void tst_wordstatsmodel::testInvalidIndices()
{
    WordsCounterModel model;
    std::vector<std::pair<QString, int>> words = {
        {"hello", 5}
    };
    model.setTopFrequentWordsList(words);
    QVERIFY(!model.data(model.index(-1, 0), WordsCounterModel::WordRole).isValid());
    QVERIFY(!model.data(model.index(1, 0), WordsCounterModel::WordRole).isValid());
}

void tst_wordstatsmodel::testRoleNames()
{
    WordsCounterModel model;
    QHash<int, QByteArray> roles = model.roleNames();
    QVERIFY(roles.contains(WordsCounterModel::WordRole));
    QVERIFY(roles.contains(WordsCounterModel::CountRole));
    QCOMPARE(roles.value(WordsCounterModel::WordRole), QByteArray("word"));
    QCOMPARE(roles.value(WordsCounterModel::CountRole), QByteArray("count"));
}

void tst_wordstatsmodel::testEmptyInput()
{
    WordsCounterModel model;
    std::vector<std::pair<QString, int>> emptyWords;
    model.setTopFrequentWordsList(emptyWords);
    QCOMPARE(model.rowCount(), 0);
    QVERIFY(!model.data(model.index(0, 0), WordsCounterModel::WordRole).isValid());
}

QTEST_MAIN(tst_wordstatsmodel)

#include "tst_wordstatsmodel.moc"
