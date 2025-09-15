#ifndef ITRANSLATORAPI_H
#define ITRANSLATORAPI_H

#include <QString>

class ITranslatorAPI // сделать гет инстанс, при смене комбобокса занулять поинтер на транслятор и инициализировать его другим транслятором
{
public:
    virtual ~ITranslatorAPI() {}
    virtual void sendMessage(const QString &userMessage) = 0;
    virtual void translate(const QString& text, const QString& sourceLang, const QString& targetLang) = 0;
    virtual QString detectLanguage(const QString& text) {
        Q_UNUSED(text);
        return {};
    }
    virtual QObject* qobject() = 0;
};

#endif // ITRANSLATORAPI_H
