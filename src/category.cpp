#include <QDebug>
#include "category.h"
#include "database.h"

Category::Category(QObject *parent) :
    QObject(parent),
    m_id(-1),
    m_useCount(0)
{
}

Category::~Category()
{
    m_id = -1;
    m_useCount = 0;
    m_name.clear();
}

qlonglong Category::id() const
{
    return m_id;
}

void Category::setId(qlonglong id)
{
    if (m_id != id)
        m_id = id;
}

QString Category::name() const
{
    return m_name;
}

void Category::setName(const QString &name)
{
    if (m_name != name)
        m_name = name;
}

quint32 Category::useCount() const
{
    return m_useCount;
}

void Category::incUseCount()
{
    ++m_useCount;
}

void Category::decUseCount()
{
    --m_useCount;
}

bool Category::load()
{
    bool loaded = false;
    QSqlQuery q;
    if (m_id != -1)
    {
        QString id(QString::number(m_id));
        q = DataBase::instance().exec("SELECT * FROM categories WHERE id=?;", { id });
        if (q.first() && q.isValid())
        {
            m_name = q.value("name").toString();
            loaded = true;
        }
    }
    else if (!m_name.isEmpty())
    {
        q = DataBase::instance().exec("SELECT * FROM categories WHERE name=?;", { m_name });
        if (q.first() && q.isValid())
        {
            m_id = q.value("id").toLongLong();
            loaded = true;
        }
    }
    return loaded;
}

bool Category::operator==(const Category &other) const
{
    return (other.id() == m_id) && (other.name() == m_name);
}
