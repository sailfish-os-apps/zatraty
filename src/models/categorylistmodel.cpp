#include "categorylistmodel.h"
#include "database.h"

CategoryListModel::CategoryListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int CategoryListModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return CategoryModel::instance().count();
}

QVariant CategoryListModel::data(const QModelIndex& index, int role) const
{
    const CategoryModel& categorylist = CategoryModel::instance();
    if (index.row() < 0 || index.row() >= categorylist.count())
        return QVariant();

    CategoryPtr category = categorylist.at(index.row());
    if (!category)
        return QVariant();

    switch (role)
    {
    case NameRole:
        return category->name();
    }

    return QVariant();
}

void CategoryListModel::refresh()
{
    CategoryModel::instance().refresh();
}

bool CategoryListModel::add(const QString& name)
{
    bool inserted = false;
    int row = CategoryModel::instance().count();
    beginInsertRows(QModelIndex(), row, row);
    inserted = CategoryModel::instance().add(name);
    endInsertRows();
    return inserted;
}

bool CategoryListModel::remove(int index)
{
    bool removed = false;
    if (Category *category = get(index))
    {
        beginRemoveRows(QModelIndex(), index, index);
        removed = CategoryModel::instance().remove(category);
        endRemoveRows();
    }
    return removed;
}

Category* CategoryListModel::get(int index)
{
    return CategoryModel::instance().at(index).data();
}

QHash<int, QByteArray> CategoryListModel::roleNames() const
{
    QHash<int, QByteArray> roles = {
        { NameRole, "name" }
    };

    return roles;
}
