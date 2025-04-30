#include "wordscountermodel.h"

WordsCounterModel::WordsCounterModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_topFrequentWordsList{}
    , m_countProgress{ 0 }
    , m_statusInfoText{"Open any text file"}
{}

int WordsCounterModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_topFrequentWordsList.count();
}

QVariant WordsCounterModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_topFrequentWordsList.size())
    {
        return QVariant();
    }

    auto entry = m_topFrequentWordsList.at(index.row());;

    switch (role)
    {
    case WordRole:
        return entry.word;
    case CountRole:
        return entry.count;
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> WordsCounterModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[WordRole] = "word";
    roles[CountRole] = "count";
    return roles;
}

void WordsCounterModel::setTopFrequentWordsList(std::vector<std::pair<QString, int>> currentList)
{
    resetModel();

    beginInsertRows(QModelIndex(), m_topFrequentWordsList.size(), m_topFrequentWordsList.size());

    for(const auto& entry : currentList)
    {
        m_topFrequentWordsList.emplace_back(WordEntry{ .word = entry.first , .count = entry.second});
    }

    endInsertRows();
}

void WordsCounterModel::resetModel()
{
    beginResetModel();
    m_topFrequentWordsList.clear();
    endResetModel();
}
