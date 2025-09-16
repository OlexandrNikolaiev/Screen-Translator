#include "BackButton.h"

BackButton::BackButton(QWidget *parent)
    : QPushButton(parent)
{
    defaultIcon = QIcon(":/icons/resources/icons/back.png");
    hoverIcon   = QIcon(":/icons/resources/icons/back_hover.png");

    setIcon(defaultIcon);
    setIconSize(QSize(21, 21));
    setMouseTracking(true);

    setFlat(true);
    update();
}

void BackButton::enterEvent(QEnterEvent *event)
{
    setIcon(hoverIcon);
    update();
    QPushButton::enterEvent(event);
}

void BackButton::leaveEvent(QEvent *event)
{
    setIcon(defaultIcon);
    update();
    QPushButton::leaveEvent(event);
}
