#ifndef DRAGGABLEFRAME_H
#define DRAGGABLEFRAME_H

#include <QFrame>
#include <QMouseEvent>
#include <windows.h>

class DraggableFrame : public QFrame
{
    Q_OBJECT

public:
    explicit DraggableFrame(QWidget* parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    void makeWindowTopMost(QWidget *w);

    QPoint startPos;
    bool dragging = false;
};

#endif // DRAGGABLEFRAME_H
