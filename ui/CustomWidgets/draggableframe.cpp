#include "draggableframe.h"
#include <QPushButton>

DraggableFrame::DraggableFrame(QWidget* parent)
    : QFrame(parent)
{
    setMouseTracking(true);
}

void DraggableFrame::makeWindowTopMost(QWidget *w) {
    HWND hwnd = reinterpret_cast<HWND>(w->winId());
    SetWindowPos(hwnd,
                 HWND_TOPMOST,
                 0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
}

void DraggableFrame::mousePressEvent(QMouseEvent* event)
{
    if (event->button() != Qt::LeftButton) {
        QFrame::mousePressEvent(event);
        return;
    }

    QWidget *w = window();
    makeWindowTopMost(w);
    w->raise();
    w->activateWindow();

    QWidget* child = childAt(event->pos());
    if (qobject_cast<QPushButton*>(child)) {
        dragging = false;
        QFrame::mousePressEvent(event);
        return;
    }

    dragging = true;
    startPos = event->globalPos();
    event->accept();
}

void DraggableFrame::mouseMoveEvent(QMouseEvent* event)
{
    if (dragging && (event->buttons() & Qt::LeftButton)) {
        QPoint delta = event->globalPos() - startPos;
        window()->move(window()->pos() + delta);
        startPos = event->globalPos();
        event->accept();
    } else {
        QFrame::mouseMoveEvent(event);
    }
}

void DraggableFrame::mouseReleaseEvent(QMouseEvent* event)
{
    if (dragging && event->button() == Qt::LeftButton) {
        dragging = false;
        event->accept();
    } else {
        QFrame::mouseReleaseEvent(event);
    }
}
