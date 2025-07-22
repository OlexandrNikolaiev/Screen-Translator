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

#ifdef Q_OS_WIN
#include <windows.h>

static HHOOK g_kbHook = nullptr;
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0 && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)) {
        return 1;
    }
    return CallNextHookEx(g_kbHook, nCode, wParam, lParam);
}
#endif

ScreenshotSnipper::ScreenshotSnipper(const QPixmap &pixmap)
    : QWidget(nullptr), screenshot(pixmap) {
    setWindowFlags(Qt::FramelessWindowHint |
                   Qt::Tool |
                   Qt::WindowStaysOnTopHint);
    setWindowState(Qt::WindowFullScreen);
    setAttribute(Qt::WA_DeleteOnClose);
    setCursor(Qt::CrossCursor);
    setFocusPolicy(Qt::StrongFocus);
    setWindowModality(Qt::ApplicationModal);

#ifdef Q_OS_WIN
    g_kbHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, GetModuleHandle(nullptr), 0);
#endif

    QPropertyAnimation *anim = new QPropertyAnimation(this, "vignetteStrength");
    anim->setDuration(300);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

ScreenshotSnipper::~ScreenshotSnipper() {
#ifdef Q_OS_WIN
    if (g_kbHook) {
        UnhookWindowsHookEx(g_kbHook);
        g_kbHook = nullptr;
    }
#endif
}

qreal ScreenshotSnipper::vignetteStrength() const {
    return m_vignetteStrength;
}

void ScreenshotSnipper::setVignetteStrength(qreal v) {
    m_vignetteStrength = v;
    update();
}

void ScreenshotSnipper::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.drawPixmap(0, 0, screenshot.scaled(size(), Qt::KeepAspectRatioByExpanding));

    QPainterPath dimPath;
    dimPath.addRect(rect());
    if (!selectionRect.isNull())
        dimPath.addRect(selectionRect.normalized());
    dimPath.setFillRule(Qt::OddEvenFill);

    painter.save();
    painter.setClipPath(dimPath);
    painter.fillRect(rect(), QColor(0, 0, 0, 80));

    qreal strength = m_vignetteStrength;
    if (strength > 0) {
        qreal radius = width() * 0.6 * strength;
        int alphaCenter = 0;
        int alphaEdge = int(150 * strength);
        QRadialGradient vignette(width() / 2, height() / 2, radius);
        vignette.setColorAt(0.0, QColor(0, 0, 0, alphaCenter));
        vignette.setColorAt(1.0, QColor(0, 0, 0, alphaEdge));
        painter.fillRect(rect(), vignette);
    }
    painter.restore();

    if (selecting || !selectionRect.isNull()) {
        painter.setBrush(Qt::NoBrush);
        painter.setPen(QPen(Qt::red, 2));
        painter.drawRect(selectionRect.normalized());
    }
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
        close();
    }
}

void ScreenshotSnipper::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        close();
    } else {
        event->accept();
    }
}
