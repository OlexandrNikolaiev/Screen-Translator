#include "draggableframe.h"


void DraggableFrame::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        startPos = event->globalPos();
        event->accept();
    }
    if (event->button() == Qt::LeftButton) {
        if (event->type() == QEvent::MouseButtonDblClick) {
            qDebug() << "Double Clikc!";
        }
    }
}

void DraggableFrame::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        const QPointF delta = event->globalPos() - startPos;
        window()->move(window()->pos() + delta.toPoint());
        startPos = event->globalPos();
        event->accept();
    }
}

