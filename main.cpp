#include "ui/mainwindow.h"

#include "src/screenshotSnipper/screenshotsnipper.h"
#include "ui/Overlay/OverlayWidget.h"
#include "src/ocr/TesseractOcrEngine.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QHotkey>
#include <QScreen>
#include <QtConcurrent>
#include <QFutureWatcher>
#include <qclipboard.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow q;

    TesseractOcrEngine ocr;

    QHotkey* hotkey = new QHotkey(QKeySequence("Alt+Shift+S"), true, &a);
    QObject::connect(hotkey, &QHotkey::activated, [&]() {
        QScreen* screen = QGuiApplication::primaryScreen();
        if (!screen) return;

        QPixmap screenshot = screen->grabWindow(0);

        ScreenshotSnipper* snipper = new ScreenshotSnipper(screenshot);
        OverlayWidget* overlay = new OverlayWidget(screenshot, snipper);
        overlay->show();
        snipper->setParent(overlay);
        snipper->resize(overlay->size());
        snipper->show();

        QObject::connect(snipper, &ScreenshotSnipper::selectedArea, [&, overlay](const QPixmap &cropped) {
            QApplication::clipboard()->setPixmap(cropped);
            overlay->close();

            auto future = QtConcurrent::run([&ocr]() {
                return ocr.recognizeTextFromClipboard();
            });

            auto watcher = new QFutureWatcher<QString>();
            QObject::connect(watcher, &QFutureWatcher<QString>::finished, [watcher, &q]() {
                QString result = watcher->result();
                if (!result.isEmpty()) {
                    qDebug() << "Recognized text:\n" << result;
                    q.show();
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

