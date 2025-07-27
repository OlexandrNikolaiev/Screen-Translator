#include "screenshotsnipper.h"

#include <QPainter>
#include <QClipboard>
#include <QApplication>
#include <QPropertyAnimation>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QRadialGradient>
#include <QScreen>
#include <QPainterPath>

ScreenshotSnipper::ScreenshotSnipper(const QPixmap &pixmap)
    : QWidget(nullptr), screenshot(pixmap) {}

ScreenshotSnipper::~ScreenshotSnipper() {}

QRect ScreenshotSnipper::getSelectionRect() {
    return selectionRect;
}

bool ScreenshotSnipper::isSelectingNow() {
    return selecting;
}

void ScreenshotSnipper::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        selecting = true;
        startPoint = event->pos();
        selectionRect = QRect(startPoint, startPoint);
        update();
    }
}

void ScreenshotSnipper::mouseMoveEvent(QMouseEvent *event) {
    if (selecting) {
        selectionRect = QRect(startPoint, event->pos());
        update();
    }
}

void ScreenshotSnipper::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && selecting) {
        selecting = false;
        selectionRect = QRect(startPoint, event->pos()).normalized();
        QPixmap cropped = screenshot.copy(selectionRect);
        QApplication::clipboard()->setPixmap(cropped);
        emit selectedArea(cropped);
    }
}

void ScreenshotSnipper::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        close();
    } else {
        event->accept();
    }
}
