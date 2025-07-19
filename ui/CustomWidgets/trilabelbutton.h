#ifndef TRILABELBUTTON_H
#define TRILABELBUTTON_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QPixmap>
#include <QMouseEvent>
#include <QEvent>
#include <QPainter>

class TriLabelButton : public QWidget
{
    Q_OBJECT

public:
    explicit TriLabelButton(QWidget *parent = nullptr);

    void setLeftText(const QString &text);
    void setRightText(const QString &text);
    void setPixmap(const QPixmap &pixmap);

    void swap();

signals:
    void clicked();

protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    QLabel *leftLabel;
    QLabel *iconLabel;
    QLabel *rightLabel;
    QHBoxLayout *layout;

    bool hovered = false;
};

#endif // TRILABELBUTTON_H
