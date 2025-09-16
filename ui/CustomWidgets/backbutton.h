#ifndef BACKBUTTON_H
#define BACKBUTTON_H

#include <QPushButton>
#include <QIcon>
#include <QEvent>
#include <QEnterEvent>

class BackButton : public QPushButton
{
    Q_OBJECT

public:
    explicit BackButton(QWidget *parent = nullptr);

protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    QIcon defaultIcon;
    QIcon hoverIcon;
};

#endif // BACKBUTTON_H
