#include "TriLabelButton.h"

TriLabelButton::TriLabelButton(QWidget *parent)
    : QWidget(parent)
{
    setCursor(Qt::PointingHandCursor);
    setStyleSheet(R"(
        QLabel {
            background-color: transparent;
            color: white;
            font-weight: bold;
        }
    )");

    QFont font("Calibri", 12);
    font.setBold(true);

    leftLabel = new QLabel(this);
    iconLabel = new QLabel(this);
    rightLabel = new QLabel(this);
    normalIcon = QPixmap(":/icons/resources/icons/swap.png").scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    hoverIcon  = QPixmap(":/icons/resources/icons/swap_hover.png").scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    iconLabel->setPixmap(normalIcon);


    leftLabel->setFont(font);
    rightLabel->setFont(font);
    leftLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    rightLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    iconLabel->setAlignment(Qt::AlignCenter);
    iconLabel->setFixedSize(32, 32);

    QGridLayout* grid = new QGridLayout(this);
    grid->setContentsMargins(5, 2, 5, 2);
    grid->setHorizontalSpacing(5);

    grid->addWidget(leftLabel, 0, 0);
    grid->addWidget(iconLabel, 0, 1, Qt::AlignCenter);
    grid->addWidget(rightLabel, 0, 2);

    grid->setColumnStretch(0, 1);
    grid->setColumnStretch(1, 0);
    grid->setColumnStretch(2, 1);

    setLayout(grid);
}

void TriLabelButton::setLeftText(const QString &text) {
    leftLabel->setText(text);
}

void TriLabelButton::setRightText(const QString &text) {
    rightLabel->setText(text);
}

void TriLabelButton::setPixmap(const QPixmap &pixmap) {
    iconLabel->setPixmap(pixmap.scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void TriLabelButton::swap()
{
    QString temp = rightLabel->text();
    rightLabel->setText(leftLabel->text());
    leftLabel->setText(temp);
}

void TriLabelButton::enterEvent(QEnterEvent *event) {
    hovered = true;
    iconLabel->setPixmap(hoverIcon);
    update();
    QWidget::enterEvent(event);
}

void TriLabelButton::leaveEvent(QEvent *event) {
    hovered = false;
    iconLabel->setPixmap(normalIcon);
    update();
    QWidget::leaveEvent(event);
}

void TriLabelButton::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton)
        emit clicked();
    QWidget::mousePressEvent(event);
}

void TriLabelButton::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    QColor bg = hovered ? QColor(60,60,60,0) : QColor(60,60,60,0);
    painter.setBrush(bg);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect(), 3, 3);
    QWidget::paintEvent(event);
}


