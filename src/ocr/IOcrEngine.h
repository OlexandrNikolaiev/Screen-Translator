#ifndef IOCR_ENGINE_H
#define IOCR_ENGINE_H

#include <QString>
#include <QObject>

class IOcrEngine : public QObject
{
    Q_OBJECT
public:
    virtual ~IOcrEngine() {}

    virtual QString recognizeTextFromClipboard() = 0;
    // recognizeTextFromFile

signals:
    void blurSignal(bool status);
};

#endif // IOCR_ENGINE_H
