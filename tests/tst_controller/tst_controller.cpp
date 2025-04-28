#include <QCoreApplication>
#include <QtTest>
#include "controller.h"

// add necessary includes here

class tst_controller : public QObject
{
    Q_OBJECT

public:
    tst_controller();
    ~tst_controller();

private:
    Controller m_controller;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_case1();
};

tst_controller::tst_controller() {}

tst_controller::~tst_controller() {}

void tst_controller::initTestCase() {}

void tst_controller::cleanupTestCase() {}

void tst_controller::test_case1() {}

QTEST_MAIN(tst_controller)

#include "tst_controller.moc"
