#ifndef WORDSCOUNTERMODEL_H
#define WORDSCOUNTERMODEL_H

#include <QAbstractListModel>

class WordsCounterModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(int countProgress READ countProgress WRITE setCountProgress NOTIFY countProgressChanged)
    Q_PROPERTY(QString statusInfoText READ statusInfoText WRITE setStatusInfoText NOTIFY statusInfoTextChanged)
    Q_PROPERTY(int length READ length WRITE setLength NOTIFY lengthChanged)

public:
    enum WordRoles {
        WordRole = Qt::UserRole + 1,
        CountRole,
        MaxWordCountRole
    };

    explicit WordsCounterModel(QObject *parent = nullptr);

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

    int length() const { return m_length; }
    void setLength(const int length) {
        if (m_length != length) {
            m_length = length;
            emit lengthChanged();
        }
    }

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

public slots:
    void setTopFrequentWordsList(std::vector<std::pair<QString, int>> currentList, int maxWordCount);
    void clearData();

signals:
    void countProgressChanged();
    void statusInfoTextChanged();
    void lengthChanged();

private:
    struct WordEntry {
        QString word;
        int count{};
        int maxWordCount{};
    };

    QList<WordEntry> m_topFrequentWordsList;

    int m_countProgress;
    QString m_statusInfoText;
    int m_length;
};

#endif // WORDSCOUNTERMODEL_H
