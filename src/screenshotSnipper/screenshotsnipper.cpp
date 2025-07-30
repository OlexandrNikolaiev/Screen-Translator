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
    : QWidget(nullptr), screenshot(pixmap) {qDebug()<<"snipper constructor";}

ScreenshotSnipper::~ScreenshotSnipper() {qDebug()<<"snipper destroyed";}

QRect ScreenshotSnipper::getSelectionRect() {
    return selectionRect;
}

bool ScreenshotSnipper::isSelectingNow() {
    return selecting;
}

void ScreenshotSnipper::mousePressEvent(QMouseEvent *event) {
    if (!canSelect) return;
    if (event->button() == Qt::LeftButton) {
        selecting = true;
        startPoint = event->pos();
        selectionRect = QRect(startPoint, startPoint);
        update();
    }
}

void ScreenshotSnipper::mouseMoveEvent(QMouseEvent *event) {
    if (!canSelect) return;

    if (selecting) {
        selectionRect = QRect(startPoint, event->pos());
        update();
    }
}

void ScreenshotSnipper::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && selecting && canSelect) {
        selecting = false;
        canSelect = false;

        selectionRect = QRect(startPoint, event->pos()).normalized();
        QPixmap cropped = screenshot.copy(selectionRect);
        QApplication::clipboard()->setPixmap(cropped);

        if (saveButton) { saveButton->deleteLater(); saveButton = nullptr; }
        if (retryButton) { retryButton->deleteLater(); retryButton = nullptr; }
        if (cancelButton) { cancelButton->deleteLater(); cancelButton = nullptr; }

        saveButton = new QPushButton("Save Area", this);
        retryButton = new QPushButton("Retry", this);
        cancelButton = new QPushButton("Cancel", this);

        saveButton->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
        retryButton->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
        cancelButton->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);

        saveButton->adjustSize();
        retryButton->adjustSize();
        cancelButton->adjustSize();

        QPoint basePos = selectionRect.bottomLeft();
        QPoint globalBasePos = this->mapToGlobal(basePos);
        QRect screenRect = QGuiApplication::primaryScreen()->availableGeometry();

        int spacing = 1;
        int totalWidth = saveButton->width() + retryButton->width() + cancelButton->width() + spacing * 2;

        int startX = globalBasePos.x();
        if (startX + totalWidth > screenRect.right()) {
            startX = screenRect.right() - totalWidth - 5;
        }

        int maxHeight = std::max({saveButton->height(), retryButton->height(), cancelButton->height()});
        int y = globalBasePos.y();
        if (y + maxHeight > screenRect.bottom()) {
            y -= maxHeight;
        }

        saveButton->move(this->mapFromGlobal(QPoint(startX, y)));
        retryButton->move(this->mapFromGlobal(QPoint(startX + saveButton->width() + spacing, y)));
        cancelButton->move(this->mapFromGlobal(QPoint(startX + saveButton->width() + spacing + retryButton->width() + spacing, y)));

        connect(saveButton, &QPushButton::clicked, this, [this, cropped]() {
            emit selectedArea(cropped);
            this->close();
        });

        connect(retryButton, &QPushButton::clicked, this, [this]() {
            if (saveButton) { saveButton->deleteLater(); saveButton = nullptr; }
            if (retryButton) { retryButton->deleteLater(); retryButton = nullptr; }
            if (cancelButton) { cancelButton->deleteLater(); cancelButton = nullptr; }

            selectionRect = QRect();
            canSelect = true;
            update();
        });

        connect(cancelButton, &QPushButton::clicked, this, [this]() {
            qDebug()<<"123";
            emit closeOverlay();
            close();
        });

        saveButton->show();
        retryButton->show();
        cancelButton->show();

        saveButton->raise();
        retryButton->raise();
        cancelButton->raise();

        saveButton->setFocus();
    }
}


