#ifndef SCREENSHOTSNIPPER_H
#define SCREENSHOTSNIPPER_H

#include <QWidget>
#include <QPixmap>
#include <QRect>
#include <QPoint>

class ScreenshotSnipper : public QWidget {
    Q_OBJECT
    Q_PROPERTY(qreal vignetteStrength READ vignetteStrength WRITE setVignetteStrength)

public:
    explicit ScreenshotSnipper(const QPixmap &pixmap);
    ~ScreenshotSnipper() override;

    qreal vignetteStrength() const;
    void setVignetteStrength(qreal v);

signals:
    void selectedArea(const QPixmap &cropped);

protected:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    QPixmap screenshot;
    QRect selectionRect;
    bool selecting = false;
    QPoint startPoint;
    qreal m_vignetteStrength = -2.0;
};

#endif // SCREENSHOTSNIPPER_H
