#include "controller.h"

Controller::Controller(QObject *parent)
    : QObject{parent}
    , m_canPause{ false }
    , m_canCancel{ false }
{}

void Controller::setFilePath(const QString& fileUrl)
{

}

void Controller::startProcessing()
{

}

void Controller::pauseProcessing()
{

}

void Controller::cancelProcessing()
{

}
