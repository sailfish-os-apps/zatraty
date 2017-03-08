#ifndef BACKUPLISTMODEL_H
#define BACKUPLISTMODEL_H

#include <QAbstractListModel>
#include <QSharedPointer>
#include <QStringList>
#include "category.h"
#include "categorymodel.h"

class BackupListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit BackupListModel(QObject* = 0);

    enum BackupRoles
    {
        NameRole = Qt::UserRole + 1,
        ViewNameRole
    };

    int rowCount(const QModelIndex& = QModelIndex()) const;
    QVariant data(const QModelIndex&, int = Qt::DisplayRole) const;

    Q_INVOKABLE void refresh();
    Q_INVOKABLE bool create();
    Q_INVOKABLE bool apply(int);
    Q_INVOKABLE bool remove(int);

protected:
    QHash<int, QByteArray> roleNames() const;
private:
    QStringList m_backups;
};
typedef QSharedPointer<BackupListModel> BackupListModelPtr;

Q_DECLARE_METATYPE(BackupListModel*)

#endif // BACKUPLISTMODEL_H
