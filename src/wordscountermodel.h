#ifndef WORDSCOUNTERMODEL_H
#define WORDSCOUNTERMODEL_H

#include <QAbstractListModel>

class WordsCounterModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(int wordsCount READ wordsCount WRITE setWordsCount NOTIFY wordsCountChanged)

public:
    enum WordRoles {
        WordRole = Qt::UserRole + 1,
        CountRole,
        MaxWordCountRole
    };

    explicit WordsCounterModel(QObject *parent = nullptr);

    int wordsCount() const { return m_wordsCount; }
    void setWordsCount(int wordsCount) {
        if (m_wordsCount != wordsCount) {
            m_wordsCount = wordsCount;
            emit wordsCountChanged();
        }
    }

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

public slots:
    void setTopFrequentWordsList(std::vector<std::pair<QString, int>> currentList, int maxWordCount);
    void clearData();

signals:
    void wordsCountChanged();

private:
    struct WordEntry {
        QString word;
        int count{};
        int maxWordCount{};
    };

    QList<WordEntry> m_topFrequentWordsList;
    int m_wordsCount;
};

#endif // WORDSCOUNTERMODEL_H
