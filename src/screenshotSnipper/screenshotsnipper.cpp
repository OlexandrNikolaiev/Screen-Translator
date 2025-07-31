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
    // if (!canSelect) {
    //     selectionRect = QRect();
    //     canSelect = true;
    //     decisionPanel->deleteLater();
    //     decisionPanel = nullptr;
    //     update();
    //     return;
    // }

    if (!canSelect) {
        emit closeOverlay();
    }

    if (event->button() == Qt::LeftButton) {
        selecting = true;
        startPoint = event->pos();
        selectionRect = QRect(startPoint, startPoint);
        qDebug()<<"1";
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

        if (selectionRect.width() == 1 || selectionRect.height() == 1) {
            // selectionRect = QRect();
            // canSelect = true;
            // update();
            emit closeOverlay();
            return;
        }

        QPixmap cropped = screenshot.copy(selectionRect);
        QApplication::clipboard()->setPixmap(cropped);

        if (decisionPanel) {
            decisionPanel->deleteLater();
            decisionPanel = nullptr;
        }

        decisionPanel = new DecisionPanel(this);
        decisionPanel->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);

        QPoint basePosX1 = selectionRect.bottomLeft();
        QPoint basePosX2 = selectionRect.bottomRight();
        QPoint basePos = (basePosX2 + basePosX1) / 2;
        QPoint globalBasePos = this->mapToGlobal(basePos);
        QRect screenRect = QGuiApplication::primaryScreen()->geometry();


        int panelWidth = decisionPanel->width();
        int panelHeight = decisionPanel->height();

        qDebug()<<"screenRect.bottom() = " << screenRect.bottom();

        int startX = globalBasePos.x() - panelWidth / 2;
        int startY = globalBasePos.y()+4;

        qDebug()<<"startY = " << startY;

        if (startX + panelWidth > screenRect.right()) {
            startX = screenRect.right() - panelWidth - 5;
        } else if (startX < screenRect.left()) {
            startX = screenRect.left() + 5;
        }

        if (startY + panelHeight > screenRect.bottom()) {
            startY -= panelHeight+5;
        }

        decisionPanel->move(this->mapFromGlobal(QPoint(startX, startY)));

        connect(decisionPanel, &DecisionPanel::confirm, this, [this, cropped]() {
            emit selectedArea(cropped);
            this->close();
        });

        connect(decisionPanel, &DecisionPanel::retry, this, [this]() {
            if (decisionPanel) {
                decisionPanel->deleteLater();
                decisionPanel = nullptr;
            }

            selectionRect = QRect();
            canSelect = true;
            update();
        });

        connect(decisionPanel, &DecisionPanel::cancel, this, [this]() {
            emit closeOverlay();
            this->close();
        });

        decisionPanel->show();
        decisionPanel->raise();
        decisionPanel->setFocus();
    }
}



