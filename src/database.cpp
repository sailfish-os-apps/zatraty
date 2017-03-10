#include "database.h"
#include "settings.h"
#include "category.h"
#include "categorymodel.h"
#include "expensemodel.h"
#include <QtConcurrent/QtConcurrent>
#include <QStandardPaths>
#include <QTemporaryFile>
#include <QFile>
#include <zlib.h>

DataBase &DataBase::instance()
{
    static DataBase dbase;
    return dbase;
}

QObject *DataBase::qinstance(QQmlEngine *, QJSEngine *)
{
    return &instance();
}

const QString& DataBase::dateFormat()
{
    static QString format("ddMMyyyy");
    return format;
}

const QString &DataBase::backupNameFormat()
{
    static QString format("yyyyMMddHHmmsszzz");
    return format;
}

DataBase::DataBase(QObject *parent) :
    QObject(parent)
{
    QString driver("QSQLITE");
    if (!QSqlDatabase::isDriverAvailable(driver))
    {
        setError(tr("Required sqlite driver not available!"));
        return;
    }

    const QString& dataLocation = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    m_backupDir = QString("%1/backups").arg(dataLocation);
    const QString& appName = Settings::instance().appName();
    m_dbName = QString("%1/%2.sqlite").arg(dataLocation).arg(appName);
    if (!QFile::exists(m_dbName))
    {
        QDir(dataLocation).mkpath(".");
        if (QFile::exists(appName))
            QFile::rename(appName, m_dbName);
    }
    m_dbase = QSqlDatabase::addDatabase(driver, appName);
    m_dbase.setDatabaseName(m_dbName);
    m_dbase.setConnectOptions("foreign_keys = ON");

    if (!m_dbase.open())
    {
        setError(m_dbase.lastError());
        return;
    }

    if (m_dbase.tables().count() == 0) init();
}

DataBase::~DataBase()
{
    if (m_dbase.isOpen())
        m_dbase.close();

    m_error.clear();
}

bool DataBase::init()
{
    for (const QString& tableName : Tables.keys())
    {
        QString columns;
        for (const auto& column : Tables.value(tableName))
        {
            if (!columns.isEmpty())
                columns += ", ";
            columns += QString("%1 %2").arg(column.first)
                                       .arg(column.second);
        }
        m_dbase.exec(QString("CREATE TABLE %1 (%2);").arg(tableName, columns));
        if (!setError(m_dbase.lastError()))
            return false;
    }

    for (const QString& categoryName : { tr("Food"), tr("Travels") })
        insertCategory(categoryName);

    emit updated();
    return true;
}

QSqlQuery DataBase::exec(const QString& queryString, const QStringList& params)
{
    QSqlQuery query(m_dbase);
    query.prepare(queryString);
    for (int i = 0; i < params.count(); ++i)
        query.bindValue(i, params[i]);

    if (!query.exec())
    {
        setError(query.lastError());
        return QSqlQuery();
    }
    return query;
}

qlonglong DataBase::insertCategory(const QString& category)
{
    QSqlQuery q = exec("INSERT INTO categories VALUES (NULL,?);", { category });
    if (q.numRowsAffected() > 0)
    {
        qlonglong id = q.lastInsertId().toLongLong();
        return id;
    }
    return -1;
}

qlonglong DataBase::insertExpense(qlonglong categoryId, const QDate& date, qreal amount, const QString& desc)
{
    QStringList params { QString::number(categoryId), date.toString(dateFormat()), QString::number(amount), desc };
    QSqlQuery q = exec("INSERT INTO expense VALUES (NULL,?,?,?,?);", params);
    if (q.numRowsAffected() > 0)
    {
        qlonglong id = q.lastInsertId().toLongLong();
        return id;
    }
    return -1;
}

bool DataBase::deleteExpense(qlonglong id)
{
    QString idStr(QString::number(id));
    QSqlQuery q = exec("DELETE FROM expense WHERE id=?;", { idStr });
    if (!setError(q.lastError()))
        return false;

    return true;
}

bool DataBase::deleteCategory(qlonglong id)
{
    QString idStr(QString::number(id));
    QSqlQuery q = exec("DELETE FROM expense WHERE category=?;", { idStr });
    if (!setError(q.lastError()))
        return false;

    q = exec("DELETE FROM categories WHERE id=?;", { idStr });
    if (!setError(q.lastError()))
        return false;

    return true;
}

bool DataBase::reset()
{
    for (const QString& tableName : Tables.keys())
    {
        m_dbase.exec(QString("DROP TABLE %1;").arg(tableName));
        if (!setError(m_dbase.lastError()))
            return false;
    }
    Settings::instance().setValue("expense_data_imported", false);

    return true;
}

QString DataBase::backupDir()
{
    return m_backupDir;
}

QString DataBase::error()
{
    return m_error;
}

void DataBase::setError(const QString& error)
{
    if (m_error != error)
    {
        m_error = error;
        qCritical() << m_error;
        emit errorChanged(error);
    }
}

bool DataBase::setError(const QSqlError& error)
{
    if (error.isValid())
    {
        if (!error.databaseText().isEmpty())
            setError(error.databaseText());
        else if (!error.driverText().isEmpty())
            setError(error.driverText());
        else
            setError(error.text());

        return false;
    }

    return true;
}

bool DataBase::restore(const QString &backupName)
{
    QString backupFileName(m_backupDir + QDir::separator() + backupName);
    if (!QFile::exists(backupFileName))
        return false;

    gzFile gzfp = gzopen(backupFileName.toStdString().c_str(), "rb9");
    if (!gzfp)
        return false;

    m_dbase.close();

    QFile dbFile(m_dbName);
    if (!dbFile.open(QFile::WriteOnly))
    {
        setError(dbFile.errorString());
        return false;
    }

    char buf[1024] = { 0 };
    qint64 len = 0;
    while ((len = gzread(gzfp, buf, sizeof(buf))) > 0)
    {
        if (dbFile.write(buf, len) == -1)
        {
            setError(dbFile.errorString());
            break;
        }
    }
    dbFile.close();
    gzclose(gzfp);

    if (!m_dbase.open())
    {
        setError(m_dbase.lastError());
        return false;
    }

    emit updated();
    return true;
}

bool DataBase::import() const
{
    return m_asyncWatcher.isRunning();
}

void DataBase::setImport(bool import)
{
    if (import)
    {
        QFuture<bool> future = QtConcurrent::run(importDataFromOldExpenseApp);
        m_asyncWatcher.setFuture(future);

        connect(&m_asyncWatcher, SIGNAL(finished()), this, SLOT(importFinished()));
        emit importChanged(true);
    }
}

bool DataBase::makeBackup()
{
    QString backupName(QDateTime::currentDateTimeUtc().toString(backupNameFormat()));
    backupName = m_backupDir + QDir::separator() + backupName + ".backup";

    QDir backupdDir(m_backupDir);
    if (!backupdDir.exists())
        backupdDir.mkpath(".");

    QTemporaryFile tempFile;
    tempFile.open();
    QString tempName(tempFile.fileName());
    tempFile.remove();

    QFile db(m_dbName);
    if (db.copy(tempName))
    {
        tempFile.setFileName(tempName);
        if (!tempFile.open())
        {
            setError(tempFile.errorString());
            return false;
        }

        gzFile gzfp = gzopen(backupName.toStdString().c_str(), "wb9");
        if (!gzfp)
        {
            tempFile.close();
            return false;
        }

        char buf[1024] = { 0 };
        qint64 len = 0;
        while ((len = tempFile.read(buf, sizeof(buf))) > 0)
        {
            if (gzwrite(gzfp, buf, (unsigned)len) != len)
            {
                setError(gzerror(gzfp, 0));
                break;
            }
        }
        tempFile.close();
        gzclose(gzfp);

        return true;
    }

    setError(db.errorString());
    return false;
}

bool DataBase::importDataFromOldExpenseApp()
{
    QString dbName("Categories");
    QString oldStoragePath(".local/share/harbour-expense/harbour-expense/QML/OfflineStorage");
    QString offlineStoragePath(QDir::homePath() + QDir::separator() + oldStoragePath);

    DataBase& db = DataBase::instance();
    QString oldDBPath;
    QDir storageDir(offlineStoragePath + QDir::separator() + "Databases");
    if (!storageDir.exists())
    {
        db.setError(tr("Cannot find local storage dir %1!").arg(storageDir.absolutePath()));
        return false;
    }

    for (QString filePath : storageDir.entryList({ "*.ini" }, QDir::Files))
    {
        QSettings file(storageDir.absoluteFilePath(filePath), QSettings::IniFormat);
        if (file.value("Name").toString() == dbName)
        {
            filePath.replace(".ini", ".sqlite");
            oldDBPath = storageDir.absoluteFilePath(filePath);
            if (!QFile::exists(oldDBPath))
                oldDBPath.clear();
            break;
        }
    }

    if (oldDBPath.isEmpty())
    {
        db.setError(tr("There is no data base with name %1!").arg(dbName));
        return false;
    }

    QSqlDatabase fromDB = QSqlDatabase::addDatabase("QSQLITE", "import_from");
    fromDB.setDatabaseName(oldDBPath);
    if (!fromDB.open())
    {
        db.setError(tr("Cannot open data base: %1!").arg(fromDB.lastError().databaseText()));
        return false;
    }

    QSqlQuery query = fromDB.exec("SELECT category FROM categories;");
    for (query.first(); query.isValid(); query.next())
        db.insertCategory(query.value(0).toString());
    CategoryModel::instance().refresh();

    db.setError("");
    query = fromDB.exec("SELECT * FROM expense;");
    for (query.first(); query.isValid(); query.next())
    {
        const QSqlRecord& rec = query.record();
        QString categoryName = rec.value("category").toString();
        qreal amount = rec.value("amount").toReal();
        QDate date = QDate::fromString(rec.value("date").toString(), "ddMMyyyy");
        QString desc = rec.value("desc").toString();
        CategoryPtr category = CategoryModel::instance().find(categoryName);
        if (category &&
                !db.insertExpense(category->id(), date, amount, desc))
        {
            fromDB.close();
            return false;
        }
    }

    fromDB.close();
    return true;
}

void DataBase::importFinished()
{
    Settings::instance().setValue("expense_data_imported", true);
    emit updated();
    emit importChanged(false);
    disconnect(&m_asyncWatcher, SIGNAL(finished()), this, SLOT(importFinished()));
}
