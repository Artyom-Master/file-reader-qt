#include "abstractworker.h"

AbstractWorker::AbstractWorker()
    : m_pausedCV{}
    , m_pausedMutex{}
    , m_paused{ false }
    , m_canceled{ false }
{

}

void AbstractWorker::pause()
{
    setPaused(true);
}

void AbstractWorker::resume()
{
    setPaused(false);
}

void AbstractWorker::cancel()
{
    m_canceled = true;
}

void AbstractWorker::setPaused(bool paused)
{
    std::lock_guard lock(m_pausedMutex);

    if(m_paused == paused)
    {
        return;
    }

    m_paused = paused;
    m_pausedCV.notify_one();
}

void AbstractWorker::callCVWait()
{
    std::unique_lock lock(m_pausedMutex);

    if(m_paused)
    {
        m_pausedCV.wait(lock, [this](){ return !m_paused; });
    }
}
