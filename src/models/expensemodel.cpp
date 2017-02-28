#include <QSqlQuery>
#include "expensemodel.h"
#include "categorymodel.h"
#include "database.h"

ExpenseModel::ExpenseModel(QObject *parent) :
    QObject(parent)
{
    refresh();
    connect(&CategoryModel::instance(), &CategoryModel::updated, this, &ExpenseModel::refresh);
    connect(&DataBase::instance(), &DataBase::updated, this, &ExpenseModel::refresh);
}

ExpenseModel& ExpenseModel::instance()
{
    static ExpenseModel list;
    return list;
}

void ExpenseModel::refresh()
{
    m_expenses.clear();

    QSqlQuery q = DataBase::instance().exec("SELECT * FROM expense;");
    for (q.first(); q.isValid(); q.next())
    {
        ExpensePtr expense(new Expense(this));
        expense->setId(q.value("id").toLongLong());
        expense->setCategory(q.value("category").toLongLong());
        {
            QString dateStr(q.value("date").toString());
            QDate date(QDate::fromString(dateStr, DataBase::dateFormat()));
            expense->setDate(date);
        }
        expense->setAmount(q.value("amount").toReal());
        expense->setDescription(q.value("desc").toString());

        m_expenses.append(expense);
    }
}

bool ExpenseModel::add(const QString& categoryName, qreal amount, const QString& desc)
{
    ExpensePtr expense(new Expense(this));
    CategoryPtr category = CategoryModel::instance().find(categoryName);
    if (!category)
    {
        qCritical() << QString("Category %1 was not found.").arg(categoryName);
        return false;
    }

    QDate date(QDate::currentDate());

    expense->setCategory(category);
    expense->setAmount(amount);
    expense->setDate(date);
    expense->setDescription(desc);

    qlonglong id = DataBase::instance().insertExpense(category->id(), date, amount, desc);
    if (id == -1)
    {
        qCritical() << "Expense was not saved.";
        return false;
    }

    expense->setId(id);
    m_expenses.append(expense);
    return true;
}

bool ExpenseModel::remove(qlonglong id)
{
    for (int i = 0; i < m_expenses.count(); ++i)
    {
        ExpensePtr expense(m_expenses.at(i));
        if (expense->id() == id)
        {
            m_expenses.removeAt(i);
            break;
        }
    }
    return DataBase::instance().deleteExpense(id);
}

bool ExpenseModel::remove(Expense *expense)
{
    if (!expense)
        return false;

    return remove(expense->id());
}

qreal ExpenseModel::totalAmount(Category *category)
{
    qreal total = 0.0;
    for (const auto& expense : m_expenses)
    {
        if (category && category->id() != expense->category()->id())
            continue;

        total += expense->amount();
    }
    return total;
}

qreal ExpenseModel::totalDateAmount(QDate thisDate, Category *category)
{
    qreal total = 0.0;
    if (!thisDate.isValid())
        thisDate = QDate::currentDate();
    for (const auto& expense : m_expenses)
    {
        if (category && category->id() != expense->category()->id())
            continue;

        const QDate& date = expense->date();
        if (date.year() == thisDate.year() &&
            date.month() == thisDate.month() &&
            date.day() == thisDate.day())
            total += expense->amount();
    }
    return total;
}

qreal ExpenseModel::totalMonthAmount(QDate thisDate, Category *category)
{
    qreal total = 0.0;
    if (!thisDate.isValid())
        thisDate = QDate::currentDate();

    for (const auto& expense : m_expenses)
    {
        if (category && category->id() != expense->category()->id())
            continue;

        const QDate& date = expense->date();
        if (date.year() == thisDate.year() &&
            date.month() == thisDate.month())
        {
            total += expense->amount();
        }
    }
    return total;
}

int ExpenseModel::count() const
{
    return m_expenses.count();
}

ExpensePtr ExpenseModel::at(int index) const
{
    return m_expenses.at(index);
}

QList<ExpensePtr> ExpenseModel::data() const
{
    return m_expenses;
}
