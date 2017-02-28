#include "settings.h"
#include <QDebug>
#include <QCoreApplication>

Settings::Settings()
{
    if (m_settings.contains("version"))
    {
        if (appVersion() != qApp->applicationVersion())
            m_settings.setValue("version", qApp->applicationVersion());
    }
    else
    {
        m_settings.setValue("version", qApp->applicationVersion());
        m_settings.setValue("expense_data_imported", false);

        QStringList currencies {"€", "£", "$", "₽", "Rs"};
        m_settings.beginWriteArray("currencies", currencies.length());
        for (int i = 0; i < currencies.length(); ++i)
        {
            m_settings.setArrayIndex(i);
            m_settings.setValue("currency", currencies.at(i));
        }
        m_settings.endArray();
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
