#ifndef DECISIONPANEL_H
#define DECISIONPANEL_H

#include <QWidget>

namespace Ui {
class DecisionPanel;
}

class DecisionPanel : public QWidget
{
    Q_OBJECT

public:
    explicit DecisionPanel(QWidget *parent = nullptr);
    ~DecisionPanel();

signals:
    void confirm();
    void retry();
    void cancel();

private:
    Ui::DecisionPanel *ui;
};

#endif // DECISIONPANEL_H
