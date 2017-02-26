#include <QSqlQuery>
#include "categorymodel.h"
#include "database.h"

CategoryModel::CategoryModel(QObject *parent) :
    QObject(parent)
{
    refresh();
}

CategoryModel &CategoryModel::instance()
{
    static CategoryModel list;
    return list;
}

void CategoryModel::refresh()
{
    m_categories.clear();

    QSqlQuery q = DataBase::instance().exec("SELECT * FROM categories;");
    for (q.first(); q.isValid(); q.next())
    {
        CategoryPtr category(new Category(this));

        category->setId(q.value("id").toLongLong());
        category->setName(q.value("name").toString());

        m_categories.append(category);
    }
    emit updated();
}

bool CategoryModel::add(const QString& name)
{
    CategoryPtr category(new Category(this));
    category->setName(name);

    qlonglong id = DataBase::instance().insertCategory(name);
    if (id == -1)
    {
        qCritical() << "Category was not saved.";
        return false;
    }

    category->setId(id);
    m_categories.append(category);
    return true;
}

CategoryPtr CategoryModel::find(qlonglong id)
{
    for (const auto& category : m_categories)
    {
        if (category->id() == id)
            return category;
    }
    return CategoryPtr(nullptr);
}

CategoryPtr CategoryModel::find(const QString& name)
{
    for (const auto& category : m_categories)
    {
        if (category->name() == name)
            return category;
    }
    return CategoryPtr(nullptr);
}

Category *CategoryModel::mostUsed()
{
    CategoryPtr mostUsedCategory;
    for (const auto& category : m_categories)
    {
        if (!mostUsedCategory ||
             mostUsedCategory->useCount() < category->useCount())
        {
            mostUsedCategory = category;
        }
    }
    return mostUsedCategory.data();
}

int CategoryModel::count() const
{
    return m_categories.count();
}

CategoryPtr CategoryModel::at(int index) const
{
    return m_categories.at(index);
}

bool CategoryModel::remove(qlonglong id)
{
    for (int i = 0; i < m_categories.count(); ++i)
    {
        if (m_categories.at(i)->id() == id)
        {
            m_categories.removeAt(i);
            break;
        }
    }
    return DataBase::instance().deleteCategory(id);
}

bool CategoryModel::remove(const QString &name)
{
    if (name.isEmpty())
        return false;

    qlonglong id = -1;
    for (int i = 0; i < m_categories.count(); ++i)
    {
        CategoryPtr category = m_categories.at(i);
        if (category->name() == name)
        {
            id = category->id();
            m_categories.removeAt(i);
            break;
        }
    }
    return DataBase::instance().deleteCategory(id);
}

bool CategoryModel::remove(Category *category)
{
    if (!category)
        return false;

    return remove(category->id());
}
