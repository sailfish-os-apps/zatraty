#ifndef EXPENSE_H
#define EXPENSE_H

#include <QAbstractItemModel>
#include <QString>
#include <QDateTime>
#include <QSharedPointer>
#include "category.h"

class Expense : public QObject
{
    Q_OBJECT
    Q_PROPERTY(CategoryPtr category READ category CONSTANT)
    Q_PROPERTY(QDate date READ date CONSTANT)
    Q_PROPERTY(qreal amount READ amount CONSTANT)
    Q_PROPERTY(QString description READ description CONSTANT)
public:
    explicit Expense(QObject* = 0);
    ~Expense();

    qlonglong id() const;
    CategoryPtr category() const;
    QDate date() const;
    qreal amount() const;
    QString description() const;

    void setId(qlonglong);
    void setCategory(CategoryPtr);
    void setCategory(qlonglong);
    void setDate(const QDate&);
    void setAmount(qreal);
    void setDescription(const QString&);

    bool load();

    bool operator==(const Expense&) const;

private:
    qlonglong m_id;
    CategoryPtr m_category;
    QDate m_date;
    qreal m_amount;
    QString m_description;
};
typedef QSharedPointer<Expense> ExpensePtr;

Q_DECLARE_METATYPE(Expense*)

#endif // EXPENSE_H
