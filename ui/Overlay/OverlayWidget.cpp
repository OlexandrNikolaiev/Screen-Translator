#include "OverlayWidget.h"

#include <QPainterPath>
#include <QPropertyAnimation>
#include <QPainter>
#include <QEvent>
#include <qevent.h>

#ifdef Q_OS_WIN
#include <windows.h>

static HHOOK g_kbHook = nullptr;
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0 && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)) {
        auto kbStruct = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
        if (kbStruct && kbStruct->vkCode == VK_ESCAPE) {
            if (g_kbHook) {
                // ???
                UnhookWindowsHookEx(g_kbHook);
                g_kbHook = nullptr;
            }
            return CallNextHookEx(nullptr, nCode, wParam, lParam);
        }
        return 1;
    }
    return CallNextHookEx(g_kbHook, nCode, wParam, lParam);
}

#endif

OverlayWidget::OverlayWidget(const QPixmap &screenshot, ScreenshotSnipper* snipper)
    : QWidget(nullptr), background(screenshot), _snipper(snipper)
{
    qDebug()<<"overlay";
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
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

OverlayWidget::~OverlayWidget()
{
#ifdef Q_OS_WIN
    if (g_kbHook) {
        UnhookWindowsHookEx(g_kbHook);
        g_kbHook = nullptr;
    }
#endif
}

qreal OverlayWidget::vignetteStrength() const {
    return m_vignetteStrength;
}

void OverlayWidget::setVignetteStrength(qreal v)
{
    m_vignetteStrength = v;
    update();
}

void OverlayWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, background.scaled(size(), Qt::KeepAspectRatioByExpanding));

    QPainterPath dimPath;
    dimPath.addRect(rect());
    if (!_snipper->getSelectionRect().isNull())
        dimPath.addRect(_snipper->getSelectionRect().normalized());
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

    if (_snipper->isSelectingNow() || !_snipper->getSelectionRect().isNull()) {
        painter.setBrush(Qt::NoBrush);
        painter.setPen(QPen(Qt::red, 2));
        painter.drawRect(_snipper->getSelectionRect().normalized());
    }
}

void OverlayWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        close();
    } else {
        QWidget::keyPressEvent(event);
    }
}
