#include "ui/mainwindow.h"

#include "src/screenshotSnipper/screenshotsnipper.h"
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

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "ScreenTranslator_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    TesseractOcrEngine ocr;

    QHotkey *hotkey = new QHotkey(QKeySequence("Alt+Shift+S"), true, &a);
    QObject::connect(hotkey, &QHotkey::activated, [&]() {
        QScreen *screen = QGuiApplication::primaryScreen();
        if (!screen) return;

        QPixmap screenshot = screen->grabWindow(0);

        ScreenshotSnipper *overlay = new ScreenshotSnipper(screenshot);
        overlay->show();

        QObject::connect(overlay, &ScreenshotSnipper::selectedArea, [&](const QPixmap &cropped) {
            QApplication::clipboard()->setPixmap(cropped);
            // async
            auto future = QtConcurrent::run([&ocr]() {
                return ocr.recognizeTextFromClipboard();
            });

            auto watcher = new QFutureWatcher<QString>(overlay);
            QObject::connect(watcher, &QFutureWatcher<QString>::finished, [watcher]() {
                QString result = watcher->result();

                if (!result.isEmpty()) {
                    qDebug() << "Recognized text:\n" << result;
                }
                else {
                    qDebug() << "Couldn't recognize the text.";
                }
                watcher->deleteLater();
            });
            watcher->setFuture(future);
        });
    });

    a.setQuitOnLastWindowClosed(true);
    return a.exec();
}
