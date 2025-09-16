#include "ui/mainwindow.h"

#include "src/screenshotSnipper/screenshotsnipper.h"
#include "ui/Overlay/OverlayWidget.h"
#include "src/ocr/TesseractOcrEngine.h"
#include "src/translator/geminitranslator.h"
#include "src/utils/SecretManager/secretmanager.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QHotkey>
#include <QScreen>
#include <QtConcurrent>
#include <QFutureWatcher>
#include <qclipboard.h>

#include "src/ocr/IOcrEngine.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow q;

    //TesseractOcrEngine ocr;

    IOcrEngine* engine = new TesseractOcrEngine();
    QObject::connect(engine, &TesseractOcrEngine::signal, &q, &MainWindow::setBlurTextEdit);

    SecretManager secrets;

    QString geminiAPI = secrets.getApiKey("GEMINI");

    auto translator = new GeminiClient(geminiAPI, &a); //test
    QObject::connect(translator, &GeminiClient::translated, &q, &MainWindow::setTargetText);
    //QObject::connect(translator, &GeminiClient::blurSignal, &q, &MainWindow::setBlurTextEdit_2);

    QHotkey* hotkey = new QHotkey(QKeySequence("Alt+Shift+S"), true, &a); // unreal engine does not work
    QObject::connect(hotkey, &QHotkey::activated, [&]() {

        QScreen* screen = QGuiApplication::primaryScreen();
        if (!screen) return;

        QPixmap screenshot = screen->grabWindow(0);

        ScreenshotSnipper* snipper = new ScreenshotSnipper(screenshot);
        OverlayWidget* overlay = new OverlayWidget(screenshot, snipper);
        QObject::connect(snipper, &ScreenshotSnipper::closeOverlay, overlay, &OverlayWidget::smoothClose);
        overlay->show();
        snipper->setParent(overlay);
        snipper->resize(overlay->size());
        snipper->show();
        qDebug()<<"vezuviy";

        QObject::connect(snipper, &ScreenshotSnipper::selectedArea, [&, overlay](const QPixmap &cropped) {
            //q.setStackedWidgetIndex(0);
            q.raise();
            q.showNormal();
            QApplication::clipboard()->setPixmap(cropped);
            overlay->close();
            auto future = QtConcurrent::run([&engine, &q]() {
                qDebug()<<"11111111111";
                return engine->recognizeTextFromClipboard();
            });

            auto watcher = new QFutureWatcher<QString>();
            qDebug()<<"222";
            QObject::connect(watcher, &QFutureWatcher<QString>::finished, [watcher, &q, translator]() {
                qDebug()<<"333";
                QString result = watcher->result();
                if (!result.isEmpty()) {
                    qDebug() << "Recognized text:\n" << result;
                    q.setSourceText(result);
                    translator->translate(result, "en", "rus");
                    if (q.getIsCollapsed()) {
                        q.raise();
                        q.showNormal();
                    } else {
                        q.show();
                        q.activateWindow();
                        q.setFocus();
                    }
                } else {
                    qDebug() << "No text recognized.";
                }
                watcher->deleteLater();
            });
            watcher->setFuture(future);
        });
    });

    a.setQuitOnLastWindowClosed(false);
    return a.exec();
}

