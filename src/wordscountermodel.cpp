#include "wordscountermodel.h"
#include <QDebug>

WordsCounterModel::WordsCounterModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_topFrequentWordsList{}
    , m_countProgress{ 0 }
    , m_statusInfoText{"Open any text file."}
    , m_length{ 0 }
{

}

int WordsCounterModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_topFrequentWordsList.count();
}

QVariant WordsCounterModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_topFrequentWordsList.size())
    {
        qDebug() << QString("index %1 is not valid").arg(index.row());
        return QVariant();
    }

    auto entry = m_topFrequentWordsList.at(index.row());;

    switch (role)
    {
        case WordRole:
        {
            return entry.word;
        }
        case CountRole:
        {
            return entry.count;
        }
        case MaxWordCountRole:
        {
            return entry.maxWordCount;
        }
        default:
        {
            return QVariant();
        }
    }
}

QHash<int, QByteArray> WordsCounterModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[WordRole] = "word";
    roles[CountRole] = "count";
    roles[MaxWordCountRole] = "maxWordCount";
    return roles;
}

void WordsCounterModel::setTopFrequentWordsList(std::vector<std::pair<QString, int>> currentList, int maxWordCount, int currentReadingProgress)
{
    if(maxWordCount == 0)
    {
        maxWordCount = 1;
    }

    clearData();
    qInfo() << "Start insertion of new wordsList in model";

    beginInsertRows(QModelIndex(), 0, currentList.size() - 1);

    for(const auto& entry : currentList)
    {
        m_topFrequentWordsList.emplace_back(WordEntry{ .word = entry.first , .count = entry.second, .maxWordCount = maxWordCount});
    }

    endInsertRows();

    setLength(m_topFrequentWordsList.count());
    setCountProgress(currentReadingProgress);
    setStatusInfoText(QString("Progress of words counting: %1%.").arg(currentReadingProgress));

    qInfo() << "Finish insertion of new wordsList in model";
}

void WordsCounterModel::clearData()
{
    qInfo() << "Reset model";

    beginResetModel();
    m_topFrequentWordsList.clear();
    endResetModel();

    setLength(0);
    setCountProgress(0);
}
