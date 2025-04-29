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
    void test_case1();
};

tst_wordstatsmodel::tst_wordstatsmodel() {}

tst_wordstatsmodel::~tst_wordstatsmodel() {}

void tst_wordstatsmodel::initTestCase() {}

void tst_wordstatsmodel::cleanupTestCase() {}

void tst_wordstatsmodel::test_case1() {}

QTEST_MAIN(tst_wordstatsmodel)

#include "tst_wordstatsmodel.moc"
