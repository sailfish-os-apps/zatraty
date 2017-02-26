#ifndef CATEGORYMODEL_H
#define CATEGORYMODEL_H

#include <QObject>
#include <QList>
#include "category.h"

class CategoryModel : public QObject
{
    Q_OBJECT
public:
    explicit CategoryModel(QObject* = 0);
    static CategoryModel &instance();

    Q_INVOKABLE bool add(const QString&);
    Q_INVOKABLE bool remove(qlonglong);
    Q_INVOKABLE bool remove(const QString&);
    Q_INVOKABLE bool remove(Category*);

    Q_INVOKABLE Category *mostUsed();

    CategoryPtr find(qlonglong);
    CategoryPtr find(const QString&);

    int count() const;
    CategoryPtr at(int) const;

private:
    Q_DISABLE_COPY(CategoryModel)

    QList<CategoryPtr> m_categories;

public slots:
    void refresh();
signals:
    void updated();
};

#endif // CATEGORYMODEL_H
