#include <QDebug>
#include "expense.h"
#include "database.h"
#include "categorymodel.h"
#include "expensemodel.h"

Expense::Expense(QObject *parent) :
    QObject(parent),
    m_id(-1),
    m_category(nullptr),
    m_amount(0.0)
{
}

Expense::~Expense()
{
    m_id = -1;
    m_amount = 0.0;
    m_date = QDate();
    m_description.clear();
    setCategory(CategoryPtr(nullptr));
}

qlonglong Expense::id() const
{
    return m_id;
}

CategoryPtr Expense::category() const
{
    return m_category;
}

QDate Expense::date() const
{
    return m_date;
}

qreal Expense::amount() const
{
    return m_amount;
}

QString Expense::description() const
{
    return m_description;
}

void Expense::setId(qlonglong id)
{
    if (id != m_id)
        m_id = id;
}

void Expense::setCategory(CategoryPtr category)
{
    if (category != m_category)
    {
        if (m_category)
            m_category->decUseCount();
        m_category = category;
        if (m_category)
            m_category->incUseCount();
    }
}

void Expense::setCategory(qlonglong categoryId)
{
    if (!m_category || categoryId != m_category->id())
    {
        CategoryPtr category = CategoryModel::instance().find(categoryId);
        if (!category)
            qCritical() << "Category (" << categoryId << ") was not found.";
        setCategory(category);
    }
}

void Expense::setDate(const QDate& date)
{
    if (date != m_date)
        m_date = date;
}

void Expense::setAmount(qreal amount)
{
    if (fabs(amount - m_amount) >= 0.01)
        m_amount = amount;
}

void Expense::setDescription(const QString& description)
{
    if (description != m_description)
        m_description = description;
}

bool Expense::load()
{
    bool loaded = false;
    if (m_id != -1)
    {
        const QString& id = QString::number(m_id);
        QSqlQuery q = DataBase::instance().exec("SELECT * FROM expense WHERE id=?;", { id });
        if (q.first() && q.isValid())
        {
            qlonglong categoryId = q.value("category").toLongLong();
            setCategory(categoryId);
            QString strDate = q.value("date").toString();
            m_date = QDate::fromString(strDate, DataBase::dateFormat());
            m_amount = q.value("amount").toReal();
            m_description = q.value("desc").toString();
            loaded = true;
        }
    }
    return loaded;
}

bool Expense::operator==(const Expense &other) const
{
    return (other.id() == m_id);
}
