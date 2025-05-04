#include <QCoreApplication>
#include <QtTest>
#include "controller.h"
#include "wordscountermodel.h"

class tst_controller : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase() {
        model = std::make_shared<WordsCounterModel>();
        ctrl = new Controller(model, nullptr);
    }
    void cleanupTestCase() {
        delete ctrl;
    }

    void testInitialFlags() {
        QCOMPARE(ctrl->canOpenFile(), true);
        QCOMPARE(ctrl->canStart(), false);
        QCOMPARE(ctrl->canPause(), false);
        QCOMPARE(ctrl->canCancel(), false);
    }

    void testOpenFileSignal() {
        QSignalSpy spy(ctrl, &Controller::openFileSignal);
        ctrl->openFile(QUrl());              // empty
        QCOMPARE(spy.count(), 0);

        QString tmp = QDir::temp().filePath("dummy.txt");
        QUrl url = QUrl::fromLocalFile(tmp);
        ctrl->openFile(url);
        QCOMPARE(spy.count(), 1);
        QCOMPARE(spy.takeFirst().at(0).toString(), tmp);
    }

    void testStartProcessingEmitsReadFile() {
        QSignalSpy spy(ctrl, &Controller::readFileSignal);
        ctrl->startProcessing();
        QCOMPARE(spy.count(), 1);

        // Flags should update
        QCOMPARE(ctrl->canOpenFile(), false);
        QCOMPARE(ctrl->canStart(), false);
        QCOMPARE(ctrl->canPause(), true);
        QCOMPARE(ctrl->canCancel(), true);
    }

    void testPauseContinueToggle() {
        ctrl->setPauseButtonText("Pause");
        ctrl->pauseProcessing();
        QCOMPARE(ctrl->pauseButtonText(), QString("Continue"));
        QCOMPARE(ctrl->canPause(), true);  // still can pause/continue

        ctrl->pauseProcessing();
        QCOMPARE(ctrl->pauseButtonText(), QString("Pause"));
    }

private:
    std::shared_ptr<WordsCounterModel> model;
    Controller *ctrl;
};

QTEST_MAIN(tst_controller)

#include "tst_controller.moc"
