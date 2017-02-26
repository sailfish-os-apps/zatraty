#ifndef CATEGORY_H
#define CATEGORY_H

#include <QAbstractListModel>
#include <QSharedPointer>

class Category : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name CONSTANT)
public:
    explicit Category(QObject* = 0);
    ~Category();

    qlonglong id() const;
    void setId(qlonglong);

    QString name() const;
    void setName(const QString&);

    void incUseCount();
    void decUseCount();
    quint32 useCount() const;

    bool load();

    bool operator==(const Category&) const;
private:
    qlonglong m_id;
    QString m_name;
    quint32 m_useCount;
};
typedef QSharedPointer<Category> CategoryPtr;

Q_DECLARE_METATYPE(Category*)

#endif // CATEGORY_H
