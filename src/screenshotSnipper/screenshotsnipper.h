#ifndef SCREENSHOTSNIPPER_H
#define SCREENSHOTSNIPPER_H

#include <QWidget>
#include <QPixmap>
#include <QRect>
#include <QPoint>

class ScreenshotSnipper : public QWidget {
    Q_OBJECT

public:
    explicit ScreenshotSnipper(const QPixmap &pixmap);
    ~ScreenshotSnipper() override;

    QRect getSelectionRect();
    bool isSelectingNow();

signals:
    void selectedArea(const QPixmap &cropped);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    QPixmap screenshot;
    QRect selectionRect;
    bool selecting = false;
    QPoint startPoint;
};

#endif // SCREENSHOTSNIPPER_H
