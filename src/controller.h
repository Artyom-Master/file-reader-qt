#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>

class Controller : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool canPause READ canPause WRITE setCanPause NOTIFY canPauseChanged)
    Q_PROPERTY(bool canCancel READ canCancel WRITE setCanCancel NOTIFY canCancelChanged)


public:
    explicit Controller(QObject *parent = nullptr);

    bool canPause() const { return m_canPause; }
    void setCanPause(bool canPause) {
        if (m_canPause != canPause) {
            m_canPause = canPause;
            emit canPauseChanged();
        }
    }

    bool canCancel() const { return m_canCancel; }
    void setCanCancel(bool canCancel) {
        if (m_canCancel != canCancel) {
            m_canCancel = canCancel;
            emit canCancelChanged();
        }
    }

public slots:
    void setFilePath(const QString& fileUrl);
    void startProcessing();
    void pauseProcessing();
    void cancelProcessing();


signals:
    void canPauseChanged();
    void canCancelChanged();

private:
    bool m_canPause;
    bool m_canCancel;

};

#endif // CONTROLLER_H
