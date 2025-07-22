#ifndef TESSERACTOCRENGINE_H
#define TESSERACTOCRENGINE_H

#include "IOcrEngine.h"

#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <windows.h>


struct Pix;

class TesseractOcrEngine : public IOcrEngine
{
public:
    TesseractOcrEngine();
    ~TesseractOcrEngine();

    QString recognizeTextFromClipboard() override;

private:
    Pix* HBitmapToPix(HBITMAP hBitmap);
    void* _ocrApi;
};

#endif // TESSERACTOCRENGINE_H
