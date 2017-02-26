#ifndef EXPENSELISTMODEL_H
#define EXPENSELISTMODEL_H

#include <QAbstractItemModel>
#include <QString>
#include "expense.h"
#include "models/expensemodel.h"

class ExpenseListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString dateFilter READ dateFilter WRITE setDateFilter NOTIFY dateFilterChanged)
    Q_PROPERTY(Category* categoryFilter READ categoryFilter WRITE setCategoryFilter NOTIFY categoryFilterChanged)
    Q_PROPERTY(bool reverse READ reverse WRITE setReverse NOTIFY reverseChanged)
public:
    explicit ExpenseListModel(QObject* = 0);

    enum ExpenseRoles
    {
        CategoryRole = Qt::UserRole + 1,
        DateRole,
        AmountRole,
        DescriptionRole
    };

    int rowCount(const QModelIndex& = QModelIndex()) const;
    QVariant data(const QModelIndex&, int = Qt::DisplayRole) const;

    Q_INVOKABLE void refresh();
    Q_INVOKABLE bool add(const QString&, qreal, const QString&);
    Q_INVOKABLE bool remove(int);
    Q_INVOKABLE Expense *get(int);

    QString dateFilter();
    void setDateFilter(const QString&);
    Q_SIGNAL void dateFilterChanged(const QString&);

    Category *categoryFilter();
    void setCategoryFilter(Category*);
    Q_SIGNAL void categoryFilterChanged(Category*);

    bool reverse();
    void setReverse(bool);
    Q_SIGNAL void reverseChanged(bool);

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    QList<ExpensePtr> m_expenses;
    Category* m_categoryFilter;
    QList<int> m_dateFilter; // [YEAR[MONTH[DAY]]]
    bool m_reverse;
};
typedef QSharedPointer<ExpenseListModel> ExpenseListModelPtr;

Q_DECLARE_METATYPE(ExpenseListModel*)

#endif // EXPENSELISTMODEL_H
