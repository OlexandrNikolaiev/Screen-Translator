#include "decisionpanel.h"
#include "ui_decisionpanel.h"

DecisionPanel::DecisionPanel(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DecisionPanel)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_TranslucentBackground);
    QObject::connect(ui->confirmButton, &QPushButton::clicked, this, &DecisionPanel::confirm);
    QObject::connect(ui->retryButton, &QPushButton::clicked, this, &DecisionPanel::retry);
    QObject::connect(ui->cancelButton, &QPushButton::clicked, this, &DecisionPanel::cancel);
}

DecisionPanel::~DecisionPanel()
{
    delete ui;
}
