#ifndef OVERLAYWIDGET_H
#define OVERLAYWIDGET_H

#include "../../src/screenshotSnipper/screenshotsnipper.h"

#include <QApplication>
#include <QWidget>
#include <QObject>

class OverlayWidget : public QWidget {
    Q_OBJECT
    Q_PROPERTY(qreal vignetteStrength READ vignetteStrength WRITE setVignetteStrength)

public:
    explicit OverlayWidget(const QPixmap& screenshot, ScreenshotSnipper* snipper);
    ~OverlayWidget();
    qreal vignetteStrength() const;
    void setVignetteStrength(qreal v);

signals:
    void selectedArea(const QPixmap &cropped);

protected:
    void paintEvent(QPaintEvent *) override;
    void keyPressEvent(QKeyEvent* event) override;

private:
    qreal m_vignetteStrength = -2.0;
    QPixmap background;

    ScreenshotSnipper* _snipper;
};

#endif // OVERLAYWIDGET_H
