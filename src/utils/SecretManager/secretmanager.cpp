// SecretManager.cpp
#include "SecretManager.h"
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>


SecretManager::SecretManager(const QString& fileName)

    : settings(fileName, QSettings::IniFormat)
{
}

void SecretManager::setApiKey(const QString& keyName, const QString& value)
{
    settings.beginGroup("API_KEYS");
    settings.setValue(keyName, value);
    settings.endGroup();
    settings.sync();
}

QString SecretManager::getApiKey(const QString& keyName)
{
    settings.beginGroup("API_KEYS");
    QString v = settings.value(keyName, QString()).toString();
    settings.endGroup();
    return v;
}
