#ifndef WORDSTATSMODEL_H
#define WORDSTATSMODEL_H

#include <QAbstractListModel>

class WordStatsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    WordStatsModel();

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    Q_INVOKABLE virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
};

#endif // WORDSTATSMODEL_H
