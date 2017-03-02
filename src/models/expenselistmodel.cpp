#include <QDebug>
#include "expenselistmodel.h"
#include "database.h"

ExpenseListModel::ExpenseListModel(QObject *parent)
    : QAbstractListModel(parent),
      m_categoryFilter(nullptr),
      m_reverse(false)
{
    refresh();
}

int ExpenseListModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return m_expenses.count();
}

QVariant ExpenseListModel::data(const QModelIndex& index, int role) const
{
    if (index.row() < 0 || index.row() >= m_expenses.count())
        return QVariant();

    ExpensePtr expense = m_expenses.at(index.row());
    if (!expense)
        return QVariant();

    switch (role)
    {
    case CategoryRole:
        return expense->category()->name();
    case DateRole:
        return expense->date();
    case AmountRole:
        return expense->amount();
    case DescriptionRole:
        return expense->description();
    }
    return QVariant();
}

void ExpenseListModel::refresh()
{
    m_expenses.clear();

    for (const auto& expense : ExpenseModel::instance().data())
    {
        if (!m_dateFilter.isEmpty())
        {
            bool satisfied = true;
            const QDate& date = expense->date();

            switch (m_dateFilter.count())
            {
            case 3:
                satisfied &= (m_dateFilter[2] == date.day());
            case 2:
                satisfied &= (m_dateFilter[1] == date.month());
            case 1:
                satisfied &= (m_dateFilter[0] == date.year());
            default:
                break;
            }
            if (!satisfied) continue;
        }

        if (m_categoryFilter &&
            m_categoryFilter->id() != expense->category()->id())
                continue;

        if (m_reverse)
            m_expenses.insert(0, expense);
        else
            m_expenses.append(expense);
    }
    emit layoutChanged();
}

bool ExpenseListModel::add(const QString& categoryName, qreal amount, const QString& descr)
{
    bool inserted = false;
    int row = m_reverse ? 0 : ExpenseModel::instance().count();
    beginInsertRows(QModelIndex(), row, row);
    inserted = ExpenseModel::instance().add(categoryName, amount, descr);
    if (inserted)
        refresh();
    endInsertRows();
    return inserted;
}

bool ExpenseListModel::remove(int index)
{
    bool removed = false;
    if (Expense *expense = get(index))
    {
        beginRemoveRows(QModelIndex(), index, index);
        removed = ExpenseModel::instance().remove(expense);
        if (removed)
            refresh();
        endRemoveRows();
    }
    return removed;
}

Expense *ExpenseListModel::get(int index)
{
    return m_expenses.at(index).data();
}

QString ExpenseListModel::dateFilter()
{
    QString result;
    for (int i = 0; i < m_dateFilter.count(); ++i)
    {
        if (i > 0)
            result += ".";
        result += QString::number(m_dateFilter[i]);
    }
    return result;
}

void ExpenseListModel::setDateFilter(const QString &dateFilter)
{
    QList<int> dateFilterList;
    if (!dateFilter.isEmpty())
    {
        for (const QString& item : dateFilter.split('.'))
            dateFilterList.append(item.toInt());

        if (dateFilterList.count() > 3)
        {
            qCritical() << QString("Date filter '%1' is incorrect").arg(dateFilter);
            return;
        }
    }

    bool filterChanged = false;
    if (dateFilterList.count() == m_dateFilter.count())
    {
        for (int i = 0; i < dateFilterList.count(); ++i)
        {
            int newItem = dateFilterList[i];
            if (newItem != m_dateFilter[i])
            {
                m_dateFilter[i] = newItem;
                filterChanged = true;
            }
        }
    }
    else
    {
        m_dateFilter = dateFilterList;
        filterChanged = true;
    }

    if (filterChanged)
    {
        emit dateFilterChanged(dateFilter);
        refresh();
    }
}

Category *ExpenseListModel::categoryFilter()
{
    return m_categoryFilter;
}

void ExpenseListModel::setCategoryFilter(Category *categoryFilter)
{
    if (m_categoryFilter != categoryFilter)
    {
        m_categoryFilter = categoryFilter;
        emit categoryFilterChanged(m_categoryFilter);
        refresh();
    }
}

bool ExpenseListModel::reverse()
{
    return m_reverse;
}

void ExpenseListModel::setReverse(bool reverse)
{
    if (m_reverse != reverse)
    {
        m_reverse = reverse;
        emit reverseChanged(m_reverse);
        refresh();
    }
}

QHash<int, QByteArray> ExpenseListModel::roleNames() const
{
    QHash<int, QByteArray> roles = {
        { CategoryRole, "category" },
        { DateRole, "date" },
        { AmountRole, "amount" },
        { DescriptionRole, "description" }
    };

    return roles;
}
