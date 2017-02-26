#include "datelistmodel.h"
#include "database.h"

DateListModel::DateListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    refresh();
    connect(&DataBase::instance(), &DataBase::updated, this, &DateListModel::refresh);
}

int DateListModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return m_dates.count();
}

QVariant DateListModel::data(const QModelIndex& index, int role) const
{
    if (index.row() < 0 || index.row() >= m_dates.count())
        return QVariant();

    const QDate& date = m_dates.at(index.row());
    switch (role)
    {
    case DateRole:
        return date;
    }
    return QVariant();
}

void DateListModel::setGroup(DateListModel::DateListGroup group)
{
    if (m_group != group)
    {
        m_group = group;
        refresh();
        emit groupChanged(m_group);
    }
}

DateListModel::DateListGroup DateListModel::group()
{
    return m_group;
}

void DateListModel::setReverse(bool reverse)
{
    if (m_reverse != reverse)
    {
        m_reverse = reverse;
        emit reverseChanged(reverse);
    }
}

bool DateListModel::reverse()
{
    return m_reverse;
}

void DateListModel::refresh()
{
    m_dates.clear();

    QSqlQuery q = DataBase::instance().exec("SELECT DISTINCT date FROM expense;");
    for (q.first(); q.isValid(); q.next())
    {
        QString dateStr(q.value(0).toString());
        QDate date(QDate::fromString(dateStr, DataBase::dateFormat()));
        if (m_group == ByMonth)
            date.setDate(date.year(), date.month(), 1);

        if (!m_dates.contains(date))
        {
            if (m_reverse)
                m_dates.insert(0, date);
            else
                m_dates.append(date);
        }
    }
}

QHash<int, QByteArray> DateListModel::roleNames() const
{
    QHash<int, QByteArray> roles = {
        { DateRole, "date" }
    };

    return roles;
}
