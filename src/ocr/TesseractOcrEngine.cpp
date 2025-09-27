#include "TesseractOcrEngine.h"

#include <QDebug>

TesseractOcrEngine::TesseractOcrEngine()
{
    tesseract::TessBaseAPI* api = new tesseract::TessBaseAPI();
    if (api->Init("assets\\tessdata\\", "eng")) { // move to another function
        qWarning() << "Error while initializing Tesseract";
        delete api;
        _ocrApi = nullptr;
    } else {
        _ocrApi = api;
        qDebug()<<"tesseract constructor";
    }
}

TesseractOcrEngine::~TesseractOcrEngine()
{
    if (_ocrApi) {
        tesseract::TessBaseAPI* api = static_cast<tesseract::TessBaseAPI*>(_ocrApi);
        api->End();
        delete api;
        _ocrApi = nullptr;
    }
}

Pix* TesseractOcrEngine::HBitmapToPix(HBITMAP hBitmap)
{
    qDebug()<<"here";
    BITMAP bmp;
    GetObject((HBITMAP)hBitmap, sizeof(BITMAP), &bmp);

    int width = bmp.bmWidth;
    int height = bmp.bmHeight;

    BITMAPINFOHEADER bi;
    ZeroMemory(&bi, sizeof(BITMAPINFOHEADER));
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = width;
    bi.biHeight = -height; // upper left corner - start
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;

    int imageSize = width * height * 4;
    std::vector<BYTE> pixels(imageSize);

    HDC hdc = GetDC(nullptr);
    if (!GetDIBits(hdc, (HBITMAP)hBitmap, 0, height, pixels.data(), (BITMAPINFO*)&bi, DIB_RGB_COLORS)) {
        ReleaseDC(nullptr, hdc);
        return nullptr;
    }
    ReleaseDC(nullptr, hdc);

    Pix* pix = pixCreate(width, height, 32);
    if (!pix) return nullptr;

    l_uint32* data = pixGetData(pix);
    int wpl = pixGetWpl(pix);

    for (int y = 0; y < height; ++y) {
        memcpy((l_uint8*)(data + y * wpl), pixels.data() + y * width * 4, width * 4);
    }

    return pix;
}

QString TesseractOcrEngine::recognizeTextFromClipboard()
{
    emit blurSignal(true);
    if (!_ocrApi) {
        qWarning() << "Tesseract API is not initizalized.";
        return QString();
    }

    if (!OpenClipboard(nullptr)) {
        qWarning() << "Couldn't open the clipboard.";
        return QString();
    }

    HBITMAP hBitmap = (HBITMAP)GetClipboardData(CF_BITMAP);
    if (!hBitmap) {
        qWarning() << "No image in the clipboard.";
        CloseClipboard();
        return QString();
    }

    Pix* image = HBitmapToPix(hBitmap);
    CloseClipboard();

    if (!image) {
        qWarning() << "Couldn't transform HBITMAP to Pix.";
        return QString();
    }

    tesseract::TessBaseAPI* api = static_cast<tesseract::TessBaseAPI*>(_ocrApi);
    api->SetImage(image);
    char* text = api->GetUTF8Text();

    QString result = QString::fromUtf8(text);

    delete[] text;
    pixDestroy(&image);

    emit blurSignal(false);
    return result;
}
