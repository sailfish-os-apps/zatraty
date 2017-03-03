#include "settings.h"
#include <QDebug>
#include <QCoreApplication>
#include <QtGlobal>

Settings::Settings()
{
    if (isFirstRun())
    {
        m_settings.setValue("version", qApp->applicationVersion());
        m_settings.setValue("expense_data_imported", false);

        updateCurrencies();
    }
    else if (isOldSettings())
    {
        if (isOldSettings("0.4"))
            updateCurrencies();

        m_settings.setValue("version", qApp->applicationVersion());
    }
}

Settings::~Settings()
{
}

Settings& Settings::instance()
{
    static Settings settings;
    return settings;
}

QObject* Settings::qinstance(QQmlEngine*, QJSEngine*)
{
    return &instance();
}

QString Settings::error()
{
    return m_error;
}

void Settings::setError(const QString& error)
{
    if (m_error != error)
    {
        m_error = error;
        emit errorChanged(error);
    }
}

QString Settings::appName() const
{
    return m_settings.applicationName();
}

QString Settings::appVersion() const
{
    return m_settings.value("version").toString();
}

QString Settings::currency()
{
    int index = m_settings.value("currency").toInt();

    int size = m_settings.beginReadArray("currencies");
    if (index >= size)
    {
        m_settings.endArray();
        setError(tr("Incorrect index of currency"));
        return QString();
    }
    m_settings.setArrayIndex(index);
    QString res = m_settings.value("currency").toString();
    m_settings.endArray();

    return res;
}

QStringList Settings::currencies()
{
    QStringList res;
    int size = m_settings.beginReadArray("currencies");
    for (int i = 0; i < size; ++i)
    {
        m_settings.setArrayIndex(i);
        res.push_back(m_settings.value("currency").toString());
    }
    m_settings.endArray();

    return res;
}

bool Settings::addCurrency(const QString& newCurrency)
{
    int size = m_settings.beginReadArray("currencies");
    for (int i = 0; i < size; ++i)
    {
        m_settings.setArrayIndex(i);
        QString currency = m_settings.value("currency").toString();
        if (currency.compare(newCurrency) == 0)
        {
            setError(tr("Currency '%s' already exists").arg(newCurrency));
            m_settings.endArray();
            return false;
        }
    }
    m_settings.endArray();

    m_settings.beginWriteArray("currencies");
    m_settings.setArrayIndex(size);
    m_settings.setValue("currency", newCurrency);
    m_settings.endArray();

    emit currenciesChanged(currencies());

    return true;
}

QVariant Settings::value(const QString &key, const QVariant &defvalue) const
{
    QVariant value = m_settings.value(key, defvalue);
    if (value.type() == QVariant::String)
    {
        QString strValue(value.toString());
        if (strValue == "true" || strValue == "false")
            return value.toBool();
    }
    return value;
}

void Settings::setValue(const QString &key, const QVariant &value)
{
    m_settings.setValue(key, value);
    if (key == "currency")
        emit currencyChanged(currency());
}

void Settings::updateCurrencies()
{
    QStringList currencies { "€", "£", "$", "₽", "₹", "¥", "₩", "₱", "฿", "₫", "₪", "Rs", "kr" };
    int size = m_settings.beginReadArray("currencies");
    for (int i = 0; i < size; ++i)
    {
        m_settings.setArrayIndex(i);
        QString currency = m_settings.value("currency").toString();
        currencies.removeAll(currency);
    }
    m_settings.endArray();

    m_settings.beginWriteArray("currencies", size + currencies.length());
    for (int i = 0; i < currencies.length(); ++i)
    {
        m_settings.setArrayIndex(i + size);
        m_settings.setValue("currency", currencies.at(i));
    }
    m_settings.endArray();
}

bool Settings::isOldSettings(const QString& version)
{
    if (isFirstRun())
        return false;

    QStringList settingsVersion = appVersion().split(".");
    QStringList appVersion = version.isNull() ? qApp->applicationVersion().split(".") : version.split(".");

    int count = qMax(appVersion.count(), settingsVersion.count());
    for (int i = 0; i < count; ++i)
    {
        int appInt = appVersion.count() > i ? appVersion.at(i).toInt() : 0;
        int setInt = settingsVersion.count() > i ? settingsVersion.at(i).toInt() : 0;
        if (appInt != setInt)
            return appInt > setInt;
    }
    return false;
}

bool Settings::isFirstRun()
{
    return !m_settings.contains("version");
}
