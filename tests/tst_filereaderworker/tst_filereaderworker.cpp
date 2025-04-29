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
    bool createFolderAndFile(const QString& folderName, const QString& fileName);

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testOpenFileSuccess();
    void testOpenFileFailure();
    void testReopenFile();

    void testReadFileWords();
    void testReadEmptyFile();
    void testReadWithoutOpen();
};

tst_filereaderworker::tst_filereaderworker() {}
tst_filereaderworker::~tst_filereaderworker() {}

bool tst_filereaderworker::createFolderAndFile(const QString& folderName, const QString& fileName) {
    QString fullPath = QCoreApplication::applicationDirPath() + "/" + folderName;

    QDir dir;
    if (!dir.exists(fullPath)) {
        if (!dir.mkpath(fullPath)) {
            qWarning() << "Failed to create directory:" << fullPath;
            return false;
        }
    }

    QFile file(fullPath + "/" + fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << "This is a test file.\n";
        file.close();
        qDebug() << "File created at:" << file.fileName();
        return true;
    } else {
        qWarning() << "Failed to create file:" << file.errorString();
        return false;
    }
}

void tst_filereaderworker::initTestCase()
{
    if(!createFolderAndFile("testdata", "sample.txt"))
    {
        exit(-1);
    }

    if(!createFolderAndFile("testdata", "first.txt"))
    {
        exit(-1);
    }

    if(!createFolderAndFile("testdata", "second.txt"))
    {
        exit(-1);
    }

    if(!createFolderAndFile("testdata", "words.txt"))
    {
        exit(-1);
    }

    if(!createFolderAndFile("testdata", "empty.txt"))
    {
        exit(-1);
    }
}

void tst_filereaderworker::cleanupTestCase() {}


void tst_filereaderworker::testOpenFileSuccess() {
    FileReaderWorker worker;
    QSignalSpy spy(&worker, &FileReaderWorker::fileOpened);

    QString testFilePath = "testdata/sample.txt";
    QFile file(testFilePath);
    QVERIFY(file.open(QIODevice::WriteOnly | QIODevice::Text));
    QTextStream out(&file);
    out << "Sample text";
    file.close();

    worker.openFile(testFilePath);
    QCOMPARE(spy.count(), 1);
}

void tst_filereaderworker::testOpenFileFailure() {
    FileReaderWorker worker;
    QSignalSpy spy(&worker, &FileReaderWorker::errorOccurred);

    QString invalidPath = "nonexistent/file.txt";
    worker.openFile(invalidPath);

    QCOMPARE(spy.count(), 1);
    QVERIFY(spy.takeFirst().at(0).toString().contains("Failed to open file"));
}

void tst_filereaderworker::testReopenFile() {
    FileReaderWorker worker;
    QSignalSpy spy(&worker, &FileReaderWorker::fileOpened);

    QString firstFilePath = "testdata/first.txt";
    QFile firstFile(firstFilePath);
    QVERIFY(firstFile.open(QIODevice::WriteOnly | QIODevice::Text));
    QTextStream out1(&firstFile);
    out1 << "First file content";
    firstFile.close();

    QString secondFilePath = "testdata/second.txt";
    QFile secondFile(secondFilePath);
    QVERIFY(secondFile.open(QIODevice::WriteOnly | QIODevice::Text));
    QTextStream out2(&secondFile);
    out2 << "Second file content";
    secondFile.close();

    worker.openFile(firstFilePath);
    worker.openFile(secondFilePath);

    QCOMPARE(spy.count(), 2);
}


void tst_filereaderworker::testReadFileWords() {
    FileReaderWorker worker;
    QSignalSpy wordSpy(&worker, &FileReaderWorker::wordRead);
    QSignalSpy finishedSpy(&worker, &FileReaderWorker::finished);

    QString testFilePath = "testdata/words.txt";
    QFile file(testFilePath);
    QVERIFY(file.open(QIODevice::WriteOnly | QIODevice::Text));
    QTextStream out(&file);
    out << "Hello world\nThis is a test";
    file.close();

    worker.openFile(testFilePath);
    worker.readFile();

    QStringList expectedWords = {"Hello", "world", "This", "is", "a", "test"};
    QCOMPARE(wordSpy.count(), expectedWords.size());

    for (int i = 0; i < expectedWords.size(); ++i) {
        QCOMPARE(wordSpy.at(i).at(0).toString(), expectedWords.at(i));
    }

    QCOMPARE(finishedSpy.count(), 1);
}

void tst_filereaderworker::testReadEmptyFile() {
    FileReaderWorker worker;
    QSignalSpy wordSpy(&worker, &FileReaderWorker::wordRead);
    QSignalSpy finishedSpy(&worker, &FileReaderWorker::finished);

    QString testFilePath = "testdata/empty.txt";
    QFile file(testFilePath);
    QVERIFY(file.open(QIODevice::WriteOnly | QIODevice::Text));
    file.close();

    worker.openFile(testFilePath);
    worker.readFile();

    QCOMPARE(wordSpy.count(), 0);
    QCOMPARE(finishedSpy.count(), 1);
}

void tst_filereaderworker::testReadWithoutOpen() {
    FileReaderWorker worker;
    QSignalSpy errorSpy(&worker, &FileReaderWorker::errorOccurred);

    worker.readFile();

    QCOMPARE(errorSpy.count(), 1);
    QVERIFY(errorSpy.takeFirst().at(0).toString().contains("File is not opened, nothing to read"));
}


QTEST_MAIN(tst_filereaderworker)

#include "tst_filereaderworker.moc"
