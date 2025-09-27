#include "geminitranslator.h"
#include "prompt.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>


GeminiClient::GeminiClient(const QString& apiKey)
    : _apiKey(apiKey)
{
    connect(&_manager, &QNetworkAccessManager::finished, this, &GeminiClient::onReplyFinished);
}

void GeminiClient::translate(const QString& text, const QString& sourceLang, const QString& targetLang)
{
    QString promptText = Prompt::defaultPromptTemplate().arg(sourceLang, targetLang, text);
    sendMessage(promptText);
}

void GeminiClient::sendMessage(const QString& userMessage)
{
    emit blurSignal(true);
    QUrl url("https://generativelanguage.googleapis.com/v1beta/models/gemini-2.0-flash:generateContent?key=" + _apiKey);
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject part;
    part["text"] = userMessage;

    QJsonArray parts;
    parts.append(part);

    QJsonObject content;
    content["parts"] = parts;

    QJsonArray contents;
    contents.append(content);

    QJsonObject root;
    root["contents"] = contents;

    QJsonDocument doc(root);
    QByteArray data = doc.toJson();

    _manager.post(request, data);
}

void GeminiClient::onReplyFinished(QNetworkReply* reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray resp = reply->readAll();

        QJsonDocument doc = QJsonDocument::fromJson(resp);
        if (!doc.isNull() && doc.isObject()) {
            QJsonObject obj = doc.object();
            QJsonArray candidates = obj["candidates"].toArray();
            if (!candidates.isEmpty()) {
                QJsonObject first = candidates[0].toObject();
                QJsonObject content = first["content"].toObject();
                QJsonArray parts = content["parts"].toArray();
                if (!parts.isEmpty()) {
                    QJsonObject textPart = parts[0].toObject();
                    QString answer = textPart["text"].toString();
                    emit translated(answer);
                    emit blurSignal(false);
                    qDebug() << "Gemini response:" << answer;
                } else {
                    qDebug() << "No parts in response";
                }
            } else {
                qDebug() << "No candidates in response";
            }
        } else {
            qDebug() << "Invalid JSON response";
        }
    } else {
        qDebug() << "Network error:" << reply->errorString();
        qDebug() << "HTTP status code:" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        qDebug() << "Error:" << reply->error() << reply->errorString();
        qDebug() << "Response body:" << reply->readAll();

    }

    reply->deleteLater();
}

