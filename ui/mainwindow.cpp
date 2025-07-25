#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "CustomWidgets/trilabelbutton.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
    setAttribute(Qt::WA_TranslucentBackground);

    connect(ui->closeButton, &QPushButton::clicked, this, &MainWindow::closeWindow);
    connect(ui->collapseButton, &QPushButton::clicked, this, &MainWindow::collapse);
    mBorderSize = 10;

    applyShadowEffect();

    QStringList languages = {
        "English", "Russian", "Ukrainian", "Polish", "German", "Belarussian", "Italian", "French", "Spanish",
        "Portuguese","Dutch",
        "Swedish", "Norwegian", "Finnish", "Danish", "Czech", "Slovak", "Hungarian", "Romanian", "Greek",
        "Turkish", "Arabic", "Hebrew", "Chinese (Simplified)", "Chinese (Traditional)", "Japanese", "Korean",
        "Hindi","Bengali", "Urdu", "Thai", "Vietnamese", "Indonesian", "Malay", "Filipino", "Swahili", "Zulu"
    };

    TriLabelButton *triBtn = new TriLabelButton(this);
    triBtn->setLeftText("Belarussian");
    triBtn->setRightText("English");
    triBtn->setPixmap(QPixmap(":/icons/resources/icons/swap.png"));
    connect(triBtn, &TriLabelButton::clicked, this, [triBtn]() {
        triBtn->swap();
    });
    ui->horizontalLayout_4->addWidget(triBtn);

    ui->translateFrom->addItems(languages);
    ui->translateTo->addItems(languages);
}

void MainWindow::applyShadowEffect()
{
    auto makeShadow = [&]() {
        auto *shadow = new QGraphicsDropShadowEffect(this);
        shadow->setBlurRadius(9);
        shadow->setOffset(1);
        shadow->setColor(QColor(0, 0, 0, 255));
        return shadow;
    };

    ui->shadowFrame->setGraphicsEffect(makeShadow());
    ui->textEdit->setGraphicsEffect(makeShadow());
    ui->textEdit_2->setGraphicsEffect(makeShadow());

    ui->copy_1Button->setGraphicsEffect(makeShadow());
    ui->copy_2Button->setGraphicsEffect(makeShadow());
    ui->translateButton->setGraphicsEffect(makeShadow());

    ui->clearButton->setGraphicsEffect(makeShadow());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeWindow()
{
    this->close();
}

bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, qint64 *result)
{
    Q_UNUSED(eventType)
    MSG *param = static_cast<MSG *>(message);

    if (param->message == WM_NCHITTEST) {
        QPoint globalPos(GET_X_LPARAM(param->lParam), GET_Y_LPARAM(param->lParam));
        QPoint localPos = mapFromGlobal(globalPos);

        int nX = localPos.x();
        int nY = localPos.y();

        if (nX >= 0 && nX < mBorderSize) {
            if (nY >= 0 && nY < mBorderSize) {
                *result = HTTOPLEFT;
            } else if (nY >= height() - mBorderSize) {
                *result = HTBOTTOMLEFT;
            } else {
                *result = HTLEFT;
            }
        } else if (nX >= width() - mBorderSize) {
            if (nY >= 0 && nY < mBorderSize) {
                *result = HTTOPRIGHT;
            } else if (nY >= height() - mBorderSize) {
                *result = HTBOTTOMRIGHT;
            } else {
                *result = HTRIGHT;
            }
        } else if (nY >= 0 && nY < mBorderSize) {
            *result = HTTOP;
        } else if (nY >= height() - mBorderSize) {
            *result = HTBOTTOM;
        } else {
            return QWidget::nativeEvent(eventType, message, result);
        }

        return true;
    }

    return QWidget::nativeEvent(eventType, message, result);
}

void MainWindow::changeEvent(QEvent *event)
{
    QMainWindow::changeEvent(event);

    if (event->type() == QEvent::ActivationChange) {
        if (!this->isActiveWindow()) {
#ifdef Q_OS_WIN
            HWND hwnd = reinterpret_cast<HWND>(this->winId());
            SetWindowPos(hwnd,
                         HWND_NOTOPMOST,
                         0, 0, 0, 0,
                         SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
#endif
        }
    }
}

void MainWindow::collapse()
{
    this->collapse();
}



