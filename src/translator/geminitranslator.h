#ifndef GEMINITRANSLATOR_H
#define GEMINITRANSLATOR_H

#include "ITranslatorAPI.h"

#include <QNetworkReply>

class GeminiClient : public ITranslatorAPI
{
    Q_OBJECT
public:
    GeminiClient(const QString& apiKey);
    void translate(const QString& text, const QString& sourceLang, const QString& targetLang) override;

    QObject* qobject() override { return this->qobject(); }

private slots:
    void onReplyFinished(QNetworkReply* reply);

private:
    void sendMessage(const QString& userMessage) override;

    QNetworkAccessManager _manager;
    QString _apiKey;
};

#endif // GEMINITRANSLATOR_H
