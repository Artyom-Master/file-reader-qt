#ifndef WORDSTATSMODEL_H
#define WORDSTATSMODEL_H

#include <QAbstractListModel>

class WordStatsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int countProgress READ countProgress WRITE setCountProgress NOTIFY countProgressChanged)
    Q_PROPERTY(QString statusInfoText READ statusInfoText WRITE setStatusInfoText NOTIFY statusInfoTextChanged)

public:
    WordStatsModel();

    int countProgress() const { return m_countProgress; }
    void setCountProgress(const int countProgress) {
        if (m_countProgress != countProgress) {
            m_countProgress = countProgress;
            emit countProgressChanged();
        }
    }

    QString statusInfoText() const { return m_statusInfoText; }
    void setStatusInfoText(const QString& statusInfoText) {
        if (m_statusInfoText != statusInfoText) {
            m_statusInfoText = statusInfoText;
            emit statusInfoTextChanged();
        }
    }

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    Q_INVOKABLE virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

signals:
    void countProgressChanged();
    void statusInfoTextChanged();

private:
    int m_countProgress;
    QString m_statusInfoText;
};

#endif // WORDSTATSMODEL_H
