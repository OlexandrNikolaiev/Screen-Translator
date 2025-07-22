#ifndef IOCR_ENGINE_H
#define IOCR_ENGINE_H

#include <QString>

class IOcrEngine
{
public:
    virtual ~IOcrEngine() {}

    virtual QString recognizeTextFromClipboard() = 0;
    // recognizeTextFromFile
};

#endif // IOCR_ENGINE_H
