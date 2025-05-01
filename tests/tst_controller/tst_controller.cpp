#include <QCoreApplication>
#include <QtTest>
#include "controller.h"
#include "wordscountermodel.h"

class tst_controller : public QObject
{
    Q_OBJECT

public:
    tst_controller();
    ~tst_controller();

//private:
    //Controller m_controller;

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testInitialProperties();
    void testCanPauseProperty();
    void testCanCancelProperty();

    void testOpenFileEmptyUrl();
    void testOpenFileValidUrl();
    void testStartProcessing();

private:
    std::shared_ptr<WordsCounterModel> m_model;
    std::unique_ptr<Controller> m_ctrl;
};

tst_controller::tst_controller() {}

tst_controller::~tst_controller() {}

void tst_controller::initTestCase()
{
    m_model = std::make_shared<WordsCounterModel>();
    m_ctrl = std::make_unique<Controller>(m_model);
}

void tst_controller::cleanupTestCase() {}

void tst_controller::testInitialProperties()
{
    // By default, canPause and canCancel should be false
    QCOMPARE(m_ctrl->canPause(),  false);
    QCOMPARE(m_ctrl->canCancel(), false);
}

void tst_controller::testCanPauseProperty()
{
    QSignalSpy spy(m_ctrl.get(), &Controller::canPauseChanged);

    // Setting to the same value should NOT emit
    m_ctrl->setCanPause(false);
    QCOMPARE(spy.count(), 0);

    // Changing from false → true should emit once
    m_ctrl->setCanPause(true);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(m_ctrl->canPause(), true);

    // Changing back → emit again
    m_ctrl->setCanPause(false);
    QCOMPARE(spy.count(), 2);
    QCOMPARE(m_ctrl->canPause(), false);
}

void tst_controller::testCanCancelProperty()
{
    QSignalSpy spy(m_ctrl.get(), &Controller::canCancelChanged);

    m_ctrl->setCanCancel(false);
    QCOMPARE(spy.count(), 0);

    m_ctrl->setCanCancel(true);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(m_ctrl->canCancel(), true);

    m_ctrl->setCanCancel(false);
    QCOMPARE(spy.count(), 2);
    QCOMPARE(m_ctrl->canCancel(), false);
}

void tst_controller::testOpenFileEmptyUrl()
{
    QSignalSpy spy(m_ctrl.get(), &Controller::openFileSignal);

    // Passing an empty QUrl should not emit
    m_ctrl->openFile(QUrl());
    QCOMPARE(spy.count(), 0);
}

void tst_controller::testOpenFileValidUrl()
{
    QSignalSpy spy(m_ctrl.get(), &Controller::openFileSignal);

    // Use a file:// URL
    const QString  path = QDir::temp().filePath("dummy.txt");
    QUrl           url  = QUrl::fromLocalFile(path);

    m_ctrl->openFile(url);

    // Must emit exactly once, with the correct local‐file path
    QCOMPARE(spy.count(), 1);
    QList<QVariant> args = spy.takeFirst();
    QCOMPARE(args.at(0).toString(), path);
}

void tst_controller::testStartProcessing()
{
    QSignalSpy spy(m_ctrl.get(), &Controller::readFileSignal);

    m_ctrl->startProcessing();
    QCOMPARE(spy.count(), 1);
}

QTEST_MAIN(tst_controller)

#include "tst_controller.moc"
