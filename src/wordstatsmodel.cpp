#include "wordstatsmodel.h"

WordStatsModel::WordStatsModel()
    : m_countProgress{ 0 }
    , m_statusInfoText{"Open any text file"}
{}

int WordStatsModel::rowCount(const QModelIndex &parent) const
{
    return 0;
}

QVariant WordStatsModel::data(const QModelIndex &index, int role) const
{
    return {};
}
