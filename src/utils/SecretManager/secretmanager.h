// SecretManager.h
#ifndef SECRETMANAGER_H
#define SECRETMANAGER_H

#include <QString>
#include <QSettings>

class SecretManager
{
public:
    SecretManager(const QString& fileName = "secrets.ini");

    void setApiKey(const QString& keyName, const QString& value);

    QString getApiKey(const QString& keyName);

private:
    QSettings settings;
};

#endif // SECRETMANAGER_H
