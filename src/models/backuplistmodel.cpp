#include <QDir>
#include <QDebug>
#include "backuplistmodel.h"
#include "database.h"

BackupListModel::BackupListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    refresh();
}

int BackupListModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return m_backups.count();
}

QVariant BackupListModel::data(const QModelIndex& index, int role) const
{
    if (index.row() < 0 || index.row() >= m_backups.count())
        return QVariant();

    QString backupName = m_backups.at(index.row());
    if (backupName.isEmpty())
        return QVariant();

    backupName = backupName.left(backupName.count() - 7); // without ext
    switch (role)
    {
    case NameRole:
        return backupName;
    case ViewNameRole:
        return QDateTime::fromString(backupName, DataBase::backupNameFormat())
                                            .toString("yyyy.MM.dd HH:mm:ss.zzz");
    }

    return QVariant();
}

void BackupListModel::refresh()
{
    QDir dir(DataBase::instance().backupDir());
    m_backups = dir.entryList({ "*.backup" }, QDir::Files, QDir::Name | QDir::Reversed);
}

bool BackupListModel::create()
{
    bool created = false;
    beginInsertRows(QModelIndex(), 0, 0);
    created = DataBase::instance().makeBackup();
    if (created)
        refresh();
    endInsertRows();
    return created;
}

bool BackupListModel::apply(int index)
{
    bool applied = false;

    beginInsertRows(QModelIndex(), 0, 0);
    applied = DataBase::instance().makeBackup();
    endInsertRows();
    if (!applied)
        return false;

    QString backupName = m_backups.at(index);
    applied = DataBase::instance().restore(backupName);
    if (applied)
    {
        refresh();
        emit layoutChanged();
    }

    return applied;
}

bool BackupListModel::remove(int index)
{
    bool removed = false;
    const QString& name = m_backups.at(index);
    QFile backupFile(DataBase::instance().backupDir() + QDir::separator() + name);
    beginRemoveRows(QModelIndex(), index, index);
    removed = backupFile.remove();
    endRemoveRows();
    if (removed)
        refresh();
    else
        qCritical() << backupFile.errorString();
    return removed;
}

QHash<int, QByteArray> BackupListModel::roleNames() const
{
    QHash<int, QByteArray> roles = {
        { NameRole, "name" },
        { ViewNameRole, "viewName" }
    };

    return roles;
}
