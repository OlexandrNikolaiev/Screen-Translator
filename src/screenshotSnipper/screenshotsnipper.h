#ifndef SCREENSHOTSNIPPER_H
#define SCREENSHOTSNIPPER_H

#include <QWidget>
#include <QPixmap>
#include <QRect>
#include <QPoint>
#include <QPushButton>
#include "../../ui/CustomWidgets/decisionpanel.h"

class ScreenshotSnipper : public QWidget {
    Q_OBJECT

public:
    explicit ScreenshotSnipper(const QPixmap &pixmap);
    ~ScreenshotSnipper() override;

    QRect getSelectionRect();
    bool isSelectingNow();

signals:
    void selectedArea(const QPixmap &cropped);
    void closeOverlay();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QPixmap screenshot;
    QRect selectionRect;
    bool selecting = false;
    QPoint startPoint;

    DecisionPanel* decisionPanel = nullptr;

    bool canSelect = true;

};

#endif // SCREENSHOTSNIPPER_H
