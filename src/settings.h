#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QQmlEngine>
#include <QSettings>
#include <QPointer>

class Settings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString error READ error WRITE setError NOTIFY errorChanged)
    Q_PROPERTY(QString appName READ appName CONSTANT)
    Q_PROPERTY(QString appVersion READ appVersion CONSTANT)
    Q_PROPERTY(QString currency READ currency NOTIFY currencyChanged)
    Q_PROPERTY(QStringList currencies READ currencies NOTIFY currenciesChanged)
public:
    Settings();
    ~Settings();

    static Settings& instance();
    static QObject* qinstance(QQmlEngine*, QJSEngine*);

    QString error();
    void setError(const QString&);

    QString appName() const;
    QString appVersion() const;

    QString currency();
    QStringList currencies();
    Q_INVOKABLE bool addCurrency(const QString&);

    Q_INVOKABLE QVariant value(const QString&, const QVariant& = QVariant()) const;
    Q_INVOKABLE void setValue(const QString&, const QVariant&);

private:
    Q_DISABLE_COPY(Settings)

    void updateCurrencies();
    bool isOldSettings(const QString& = QString());
    bool isFirstRun();

    QSettings m_settings;
    QString m_error;

signals:
    void errorChanged(QString);
    void currencyChanged(QString);
    void currenciesChanged(QStringList);
};

#endif // SETTINGS_H
