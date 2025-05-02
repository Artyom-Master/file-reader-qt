#include <QCoreApplication>
#include <QtTest>
#include "filereaderworker.h"

// add necessary includes here

class tst_filereaderworker : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase() {
        worker = new FileReaderWorker;
    }
    void cleanupTestCase() {
        delete worker;
    }

    void testOpenFileFailure() {
        QSignalSpy errSpy(worker, &FileReaderWorker::errorOccurred);
        worker->openFile("/non/existent/path.txt");
        QCOMPARE(errSpy.count(), 1);
    }

    void testOpenFileSuccess() {
        QTemporaryFile tmp;
        tmp.open();
        tmp.write("hello world");
        tmp.close();

        QSignalSpy openSpy(worker, &FileReaderWorker::fileOpened);
        worker->openFile(tmp.fileName());
        QCOMPARE(openSpy.count(), 1);
        // argument is just the file's base name
        QCOMPARE(openSpy.takeFirst().at(0).toString(),
                 QFileInfo(tmp.fileName()).fileName());
    }

    void testReadWordsAndProgress() {
        // create a temp file with known content
        QTemporaryFile tmp;
        tmp.open();
        tmp.write("One two three");
        tmp.close();

        worker->openFile(tmp.fileName());

        QSignalSpy finishSpy(worker, &FileReaderWorker::finished);
        // ensure no errors
        QSignalSpy errSpy(worker, &FileReaderWorker::errorOccurred);

        // call start() to read
        worker->start();

        QCOMPARE(errSpy.count(), 0);
        QCOMPARE(finishSpy.count(), 1);

        // getReadWords after running; since we consumed in run(), expect no leftover
        auto progress = worker->getReadWords();
        QStringList words = progress.readWords;
        QCOMPARE(progress.readingProgress, 100);
        // words could be in any order, but should contain at least the tokens
        QVERIFY(words.contains("One"));
        QVERIFY(words.contains("two"));
        QVERIFY(words.contains("three"));
    }
/*
    void testExtractWords() {
        QStringList list = FileReaderWorker{}.extractWords("hi! 123go_bye");
        // only letter sequences
        QCOMPARE(list, QStringList({"hi","go","bye"}));
    }
*/
private:
    FileReaderWorker *worker;
};

QTEST_MAIN(tst_filereaderworker)

#include "tst_filereaderworker.moc"
