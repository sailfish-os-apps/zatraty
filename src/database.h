#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QtSql>
#include <QMap>
#include <QVector>
#include <QString>
#include <QQmlEngine>
#include <QFutureWatcher>

class DataBase : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString error READ error WRITE setError NOTIFY errorChanged)
    Q_PROPERTY(bool importation READ import WRITE setImport NOTIFY importChanged)
public:
    explicit DataBase(QObject* = 0);
    ~DataBase();

    static DataBase& instance();
    static QObject *qinstance(QQmlEngine*, QJSEngine*);
    static const QString &dateFormat();
    static const QString &backupNameFormat();

    QString backupDir();

    QSqlQuery exec(const QString&, const QStringList& = QStringList());
    qlonglong insertCategory(const QString&);
    qlonglong insertExpense(qlonglong, const QDate&, qreal, const QString& = QString());
    bool deleteCategory(qlonglong);
    bool deleteExpense(qlonglong);
    bool updateCategory(qlonglong, const QString&);

    QString error();
    void setError(const QString&);
    bool setError(const QSqlError&);
    Q_SIGNAL void errorChanged(const QString&);

    bool restore(const QString&);
    bool import() const;
    void setImport(bool);
    Q_SIGNAL void importChanged(bool);
    static bool importDataFromOldExpenseApp();

    Q_INVOKABLE bool init();
    Q_INVOKABLE bool reset();
    Q_INVOKABLE bool makeBackup();

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

    QString m_dbName;
    QString m_backupDir;
    QString m_error;
    QSqlDatabase m_dbase;
    QFutureWatcher<bool> m_asyncWatcher;

signals:
    void updated();
private slots:
    void importFinished();
};

#endif // DATABASE_H
