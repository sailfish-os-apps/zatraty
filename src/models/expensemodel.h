#ifndef EXPENSEMODEL_H
#define EXPENSEMODEL_H

#include <QObject>
#include <QString>
#include <QList>
#include "expense.h"
#include "category.h"

class ExpenseModel : public QObject
{
    Q_OBJECT
public:
    explicit ExpenseModel(QObject *parent = 0);
    static ExpenseModel& instance();

    Q_INVOKABLE bool add(const QString&, qreal, const QString&, const QDate& = QDate());
    Q_INVOKABLE bool remove(qlonglong);
    Q_INVOKABLE bool remove(Expense*);

    Q_INVOKABLE qreal totalAmount(Category* = nullptr);
    Q_INVOKABLE qreal totalDateAmount(QDate = QDate(), Category* = nullptr);
    Q_INVOKABLE qreal totalMonthAmount(QDate = QDate(), Category* = nullptr);

    int count() const;
    ExpensePtr at(int) const;
    QList<ExpensePtr> data() const;

private:
    Q_DISABLE_COPY(ExpenseModel)

    QList<ExpensePtr> m_expenses;

public slots:
    void refresh();
};

#endif // EXPENSEMODEL_H
