#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QtSql>
#include <QMap>
#include <QVector>
#include <QString>
#include <QQmlEngine>

class DataBase : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString error READ error WRITE setError NOTIFY errorChanged)
public:
    explicit DataBase(QObject* = 0);
    ~DataBase();

    static DataBase& instance();
    static QObject *qinstance(QQmlEngine*, QJSEngine*);
    static QString& dateFormat();

    QSqlQuery exec(const QString&, const QStringList& = QStringList());
    qlonglong insertCategory(const QString&);
    qlonglong insertExpense(qlonglong, const QDate&, qreal, const QString& = QString());
    bool deleteCategory(qlonglong);
    bool deleteExpense(qlonglong);

    QString error();
    void setError(const QString&);
    bool setError(const QSqlError&);
    Q_SIGNAL void errorChanged(const QString&);

    Q_INVOKABLE bool init();
    Q_INVOKABLE bool reset();
    Q_INVOKABLE bool importDataFromOldExpenseApp();

    const QMap<QString, QVector<QPair<QString, QString>>> Tables = {
        {"expense", {
             {"id", "INTEGER PRIMARY KEY AUTOINCREMENT"},
             {"category", "INTEGER REFERENCES categories(id) ON DELETE CASCADE"},
             {"date", "TEXT NOT NULL"},
             {"amount", "REAL NOT NULL"},
             {"desc", "TEXT"}
        }},
        {"categories", {
             {"id", "INTEGER PRIMARY KEY AUTOINCREMENT"},
             {"name", "TEXT NOT NULL UNIQUE"}
        }}
    };

private:
    Q_DISABLE_COPY(DataBase)

    QString m_error;
    QSqlDatabase m_dbase;

signals:
    void updated();
};

#endif // DATABASE_H
