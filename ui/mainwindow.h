#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QFrame>
#include <QGraphicsDropShadowEffect>
#include <windows.h>
#include <windowsx.h>
#include <dwmapi.h>
#include <QPainter>
#include <QTextEdit>

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


protected:
    bool nativeEvent(const QByteArray &eventType, void *message, qint64 *result) override;
    void changeEvent(QEvent* event) override;

private slots:
    void copyFromTextEdit();

private:
    void collapse();
    void clear();

    Ui::MainWindow *ui;
    int mBorderSize;


};

#endif // MAINWINDOW_H
