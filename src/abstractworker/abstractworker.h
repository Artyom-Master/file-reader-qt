#ifndef ABSTRACTWORKER_H
#define ABSTRACTWORKER_H

#include <atomic>
#include <condition_variable>

class AbstractWorker
{
public:
    AbstractWorker();

    void pause();
    void resume();
    void cancel();

    virtual void run() = 0;

protected:
    void setPaused(bool paused);
    void callCVWait();

    std::condition_variable m_pausedCV;
    std::mutex m_pausedMutex;
    std::atomic<bool> m_paused;
    std::atomic<bool> m_canceled;
};

#endif // ABSTRACTWORKER_H
