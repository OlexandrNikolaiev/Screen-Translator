#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QFrame>
#include <QGraphicsDropShadowEffect>
#include <QLabel>
#include <windows.h>
#include <windowsx.h>
#include <dwmapi.h>
#include <QPainter>
#include <QTextEdit>

#include "../src/utils/enums/enums.h"

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
    ~MainWindow();
    void applyShadowEffect();
    void closeWindow();

    void setSourceText(QString text);
    void setTargetText(QString text);

    bool getIsCollapsed() {return isCollapsed;}
    void setStackedWidgetIndex(int i);

public slots:
    void setBlurTextEdit(bool status);
    void setBlurTextEdit_2(bool status);


protected:
    bool nativeEvent(const QByteArray &eventType, void *message, qint64 *result) override;
    void changeEvent(QEvent* event) override;

private slots:
    void copyFromTextEdit();
    void on_translateButton_clicked();

private:
    bool isTranslated = false;

    void collapse();
    void clear();

    Ui::MainWindow *ui;
    int mBorderSize;

    bool eventFilter(QObject* watched, QEvent* event) override;
    void positionOverlay();

    bool isCollapsed = false;

    QGraphicsBlurEffect* blur;
    QLabel* gifOverlay;
    QMovie* loadingSpinner;

};

#endif // MAINWINDOW_H
