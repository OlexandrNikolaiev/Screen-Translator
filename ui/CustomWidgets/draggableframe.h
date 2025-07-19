#ifndef DRAGGABLEFRAME_H
#define DRAGGABLEFRAME_H
#include <QFrame>
#include <QMouseEvent>

class DraggableFrame : public QFrame
{
public:
    DraggableFrame(QWidget* parent = nullptr) {}

protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);

private:
    QPoint startPos;
    QPointF m_mousePoint;
};

#endif // DRAGGABLEFRAME_H
