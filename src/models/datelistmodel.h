#ifndef DATELISTMODEL_H
#define DATELISTMODEL_H

#include <QAbstractListModel>
#include <QSharedPointer>
#include <QList>
#include <QDate>

class DateListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(DateListGroup group READ group WRITE setGroup NOTIFY groupChanged)
    Q_PROPERTY(bool reverse READ reverse WRITE setReverse NOTIFY reverseChanged)
public:
    DateListModel(QObject* = 0);

    enum DateRoles
    {
        DateRole = Qt::UserRole + 1
    };

    int rowCount(const QModelIndex& = QModelIndex()) const;
    QVariant data(const QModelIndex&, int = Qt::DisplayRole) const;

    enum DateListGroup
    {
        None = 0,
        ByMonth
    };
    Q_ENUMS(DateListGroup)
    void setGroup(DateListGroup);
    DateListGroup group();
    Q_SIGNAL void groupChanged(DateListGroup);

    void setReverse(bool);
    bool reverse();
    Q_SIGNAL void reverseChanged(bool);

protected:
    QHash<int, QByteArray> roleNames() const;
private:
    QList<QDate> m_dates;
    DateListGroup m_group;
    bool m_reverse;
public slots:
    void refresh();
};
typedef QSharedPointer<DateListModel> DateListModelPtr;

Q_DECLARE_METATYPE(DateListModel*)

#endif // DATELISTMODEL_H
