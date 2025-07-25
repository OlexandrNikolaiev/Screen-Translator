#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QFrame>
#include <QGraphicsDropShadowEffect>
#include <windows.h>
#include <windowsx.h>
#include "Overlay/OverlayWidget.h"
#include <dwmapi.h>
#include <QPainter>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void applyShadowEffect();
    ~MainWindow();
    void closeWindow();
    OverlayWidget* getOverlay() const;

protected:
    bool nativeEvent(const QByteArray &eventType, void *message, qint64 *result) override;
    void changeEvent(QEvent* event) override;

private:
    void collapse();

    Ui::MainWindow *ui;
    int mBorderSize;
    OverlayWidget *overlay;

};

#endif // MAINWINDOW_H
