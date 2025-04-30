#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "Controller.h"
#include "wordscountermodel.h"
#include <stdio.h>
#include <stdlib.h>

QtMessageHandler originalHandler = nullptr;

void logToFile(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString message = qFormatLogMessage(type, context, msg);
    static FILE *logFile = fopen((QDateTime::currentDateTime().toString("yyyy-MM-dd") + ".txt").toStdString().c_str(), "a");

    if(logFile)
    {
        fprintf(logFile, "%s\n", qPrintable(message));
    }

    fflush(logFile);

    if (originalHandler)
    {
        originalHandler(type, context, msg);
    }
}

int main(int argc, char *argv[])
{
    qSetMessagePattern("[%{time yyyy-MM-dd hh:mm:ss.zzz}] - %{type} - %{qthreadptr} - %{file} - %{line}: %{message}");
    originalHandler = qInstallMessageHandler(logToFile);

    QGuiApplication app(argc, argv);
    QCoreApplication::setAttribute(Qt::AA_QtQuickUseDefaultSizePolicy);

    Controller controller;
    WordsCounterModel wordsCounterModel;

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("controller", &controller);
    engine.rootContext()->setContextProperty("wordsCounterModel", &wordsCounterModel);

    const QUrl url(QStringLiteral("qrc:/file-reader-qt/qml/Main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    qInfo() << "Application started";

    return app.exec();
}
