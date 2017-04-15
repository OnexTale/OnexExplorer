#include "SingleModelPreview.h"

SingleModelPreview::SingleModelPreview(QObject *parent)
    : QAbstractItemModel(parent)
{
}

QVariant SingleModelPreview::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
}

QModelIndex SingleModelPreview::index(int row, int column, const QModelIndex &parent) const
{
    // FIXME: Implement me!
}

QModelIndex SingleModelPreview::parent(const QModelIndex &index) const
{
    // FIXME: Implement me!
}

int SingleModelPreview::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    // FIXME: Implement me!
}

int SingleModelPreview::columnCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    // FIXME: Implement me!
}

QVariant SingleModelPreview::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    return QVariant();
}
