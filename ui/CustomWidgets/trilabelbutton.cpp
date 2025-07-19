#include "trilabelbutton.h"
#include "ui_trilabelbutton.h"

TriLabelButton::TriLabelButton(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TriLabelButton)
{
    ui->setupUi(this);
}

TriLabelButton::~TriLabelButton()
{
    delete ui;
}

void TriLabelButton::setFullStyle(const QString &bgColor)
{
    QString style = QString(
        "QWidget {"
        "background-color: rgb(60,60,60);"
        "color: white;"
        "}"
        "QLabel {"
        "background-color: transparent;"
        "}"
        "QFrame {"
        "background-color: transparent;"
        "}"
        );


    this->setStyleSheet(style);
}

void TriLabelButton::enterEvent(QEnterEvent *event)
{
    setFullStyle("rgb(60,60,60)");
    QWidget::enterEvent(event);
}

void TriLabelButton::leaveEvent(QEvent *event)
{
    this->setStyleSheet(R"(
        QWidget {
            background-color: rgb(46, 47, 48);
            color: white;
            border: none;
        }
    )");
    QWidget::leaveEvent(event);
}
