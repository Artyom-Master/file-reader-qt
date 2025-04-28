#include <QCoreApplication>
#include <QtTest>
#include "filereaderworker.h"

// add necessary includes here

class tst_filereaderworker : public QObject
{
    Q_OBJECT

public:
    tst_filereaderworker();
    ~tst_filereaderworker();

private:
    FileReaderWorker m_fileReaderWorker;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_case1();
};

tst_filereaderworker::tst_filereaderworker() {}

tst_filereaderworker::~tst_filereaderworker() {}

void tst_filereaderworker::initTestCase() {}

void tst_filereaderworker::cleanupTestCase() {}

void tst_filereaderworker::test_case1()
{

}

QTEST_MAIN(tst_filereaderworker)

#include "tst_filereaderworker.moc"
