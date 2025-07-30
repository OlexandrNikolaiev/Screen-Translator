#ifndef GEMINITRANSLATOR_H
#define GEMINITRANSLATOR_H

#include "ITranslatorAPI.h"

#include <QNetworkReply>

class GeminiClient : public QObject, public ITranslatorAPI
{
    Q_OBJECT
public:
    GeminiClient(const QString& apiKey, QObject* parent = nullptr);
    void translate(const QString& text, const QString& sourceLang, const QString& targetLang) override;

    QObject* qobject() override { return this; }

private slots:
    void onReplyFinished(QNetworkReply* reply);

signals:
    void translated(QString);

private:
    void sendMessage(const QString& userMessage) override;

    QNetworkAccessManager _manager;
    QString _apiKey;
};

#endif // GEMINITRANSLATOR_H
