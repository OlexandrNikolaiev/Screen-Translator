#ifndef TRILABELBUTTON_H
#define TRILABELBUTTON_H

#include <QWidget>

namespace Ui {
class TriLabelButton;
}

class TriLabelButton : public QWidget
{
    Q_OBJECT

public:
    explicit TriLabelButton(QWidget *parent = nullptr);
    ~TriLabelButton();

private:
    Ui::TriLabelButton *ui;
    void setFullStyle(const QString &bgColor);

protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;

};

#endif // TRILABELBUTTON_H
