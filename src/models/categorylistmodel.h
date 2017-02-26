#ifndef CATEGORYLISTMODEL_H
#define CATEGORYLISTMODEL_H

#include <QAbstractListModel>
#include <QSharedPointer>
#include "category.h"
#include "categorymodel.h"

class CategoryListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit CategoryListModel(QObject* = 0);

    enum CategoryRoles
    {
        NameRole = Qt::UserRole + 1
    };

    int rowCount(const QModelIndex& = QModelIndex()) const;
    QVariant data(const QModelIndex&, int = Qt::DisplayRole) const;

    Q_INVOKABLE void refresh();
    Q_INVOKABLE bool add(const QString&);
    Q_INVOKABLE bool remove(int);
    Q_INVOKABLE Category* get(int);

protected:
    QHash<int, QByteArray> roleNames() const;
};
typedef QSharedPointer<CategoryListModel> CategoryListModelPtr;

Q_DECLARE_METATYPE(CategoryListModel*)

#endif // CATEGORYLISTMODEL_H
