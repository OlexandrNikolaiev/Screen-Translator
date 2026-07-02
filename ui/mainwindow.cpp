#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "CustomWidgets/trilabelbutton.h"
#include <QClipboard>
#include <QMovie>
#include <QStackedLayout>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
    setAttribute(Qt::WA_TranslucentBackground);

    connect(ui->closeButton, &QPushButton::clicked, this, &MainWindow::closeWindow);
    connect(ui->collapseButton, &QPushButton::clicked, this, &MainWindow::collapse);
    connect(ui->closeButtonSettings, &QPushButton::clicked, this, &MainWindow::closeWindow);
    connect(ui->collapseButtonSettings, &QPushButton::clicked, this, &MainWindow::collapse);

    connect(ui->clearButton, &QPushButton::clicked, this, &MainWindow::clear);
    mBorderSize = 10;

    applyShadowEffect();

    QStringList languages = {
        "English", "Russian", "Ukrainian", "Polish", "German", "Belarussian", "Italian", "French", "Spanish",
        "Portuguese","Dutch",
        "Swedish", "Norwegian", "Finnish", "Danish", "Czech", "Slovak", "Hungarian", "Romanian", "Greek",
        "Turkish", "Arabic", "Hebrew", "Chinese (Simplified)", "Chinese (Traditional)", "Japanese", "Korean",
        "Hindi","Bengali", "Urdu", "Thai", "Vietnamese", "Indonesian", "Malay", "Filipino", "Swahili", "Zulu"
    };
    QStringList languages2 = {
        "Spanish", "Russian", "Ukrainian", "Polish", "German", "Belarussian", "Italian", "French", "Spanish",
        "Portuguese","Dutch",
        "Swedish", "Norwegian", "Finnish", "Danish", "Czech", "Slovak", "Hungarian", "Romanian", "Greek",
        "Turkish", "Arabic", "Hebrew", "Chinese (Simplified)", "Chinese (Traditional)", "Japanese", "Korean",
        "Hindi","Bengali", "Urdu", "Thai", "Vietnamese", "Indonesian", "Malay", "Filipino", "Swahili", "Zulu"
    };

    // TriLabelButton *triBtn = new TriLabelButton(this);
    // triBtn->setLeftText("English");
    // triBtn->setRightText("Spanish");
    // triBtn->setPixmap(QPixmap(":/icons/resources/icons/swap.png"));
    // connect(triBtn, &TriLabelButton::clicked, this, [triBtn]() {
    //     triBtn->swap();
    // });
    // ui->horizontalLayout_4->addWidget(triBtn);

    m_triBtn = new TriLabelButton(this);
    m_triBtn->setPixmap(QPixmap(":/icons/resources/icons/swap.png"));
    ui->horizontalLayout_4->addWidget(m_triBtn);

    ui->translateFrom->addItems(languages);
    ui->translateTo->addItems(languages2);

    ui->translateFrom->insertItem(0, "Auto Detect");

    ui->translateFrom->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    ui->translateTo->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    ui->translateFrom->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    ui->translateTo->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

    connect(ui->translateFrom, &QComboBox::currentTextChanged, this, [this](const QString &text) {
        if (m_triBtn) m_triBtn->setLeftText(text);
    });
    connect(ui->translateTo, &QComboBox::currentTextChanged, this, [this](const QString &text) {
        if (m_triBtn) m_triBtn->setRightText(text);
    });

    ui->translateFrom->setCurrentText("Auto Detect");
    ui->translateTo->setCurrentText("Ukrainian");

    // connect(m_triBtn, &TriLabelButton::clicked, this, [this]() {
    //     QString temp = ui->translateFrom->currentText();
    //     ui->translateFrom->setCurrentText(ui->translateTo->currentText());
    //     ui->translateTo->setCurrentText(temp);
    // });

    connect(ui->translateFrom, QOverload<int>::of(&QComboBox::activated), this, [this](int /*index*/) {
        ui->translateFrom->setItemText(0, "Auto Detect");

        if (ui->translateFrom->currentIndex() == 0 && m_triBtn) {
            m_triBtn->setLeftText("Auto Detect");
        }
    });

    connect(ui->translateTo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int /*index*/) {
        emit retranslateRequested();
    });

    connect(m_triBtn, &TriLabelButton::clicked, this, &MainWindow::handleSwap);

    connect(ui->copy_1Button, &QPushButton::clicked, this, &MainWindow::copyFromTextEdit);
    connect(ui->copy_2Button, &QPushButton::clicked, this, &MainWindow::copyFromTextEdit);

    connect(ui->settingsButton, &QPushButton::clicked, this, [this] {
        if(ui->stackedWidget->slideInNext()){

        }
    });
    connect(ui->backButton, &QPushButton::clicked, this, [this] {
        if(ui->stackedWidget->slideInPrev()){

        }
    });

    connect(ui->translateButton, &QPushButton::clicked, this, [this] {
        emit retranslateRequested();
    });

    gifOverlay = new QLabel(this);
    loadingSpinner = new QMovie(":/icons/resources/icons/spinner.gif", QByteArray(), this);

    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::applyShadowEffect()
{
    auto makeShadow = [&]() {
        auto *shadow = new QGraphicsDropShadowEffect(this);
        shadow->setBlurRadius(9);
        shadow->setOffset(0);
        shadow->setColor(QColor(0, 0, 0, 255));
        return shadow;
    };

    ui->stackedWidget->setGraphicsEffect(makeShadow());
    //ui->shadowFrame->setGraphicsEffect(makeShadow());
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

void MainWindow::setSourceText(QString text)
{
    ui->textEdit->setText(text);
}

void MainWindow::setTargetText(QString text)
{
    ui->textEdit_2->setText(text);
}

void MainWindow::setStackedWidgetIndex(int i)
{
    ui->stackedWidget->setCurrentIndex(i);
}

void MainWindow::setDetectedLanguageUI(const QString& lang)
{
    QString cleanLang = lang.trimmed();

    if (ui->translateFrom->currentIndex() == 0) {
        QString autoText = "Auto Detect (" + cleanLang + ")";
        ui->translateFrom->setItemText(0, autoText);
        if (m_triBtn) {
            m_triBtn->setLeftText(autoText);
        }
    }
    else {
        if (ui->translateFrom->currentText().compare(cleanLang, Qt::CaseInsensitive) != 0) {

            int index = -1;
            for (int i = 0; i < ui->translateFrom->count(); ++i) {
                if (ui->translateFrom->itemText(i).compare(cleanLang, Qt::CaseInsensitive) == 0) {
                    index = i;
                    break;
                }
            }

            if (index != -1) {
                ui->translateFrom->setCurrentIndex(index);
            }
        }
    }
}

QString MainWindow::getSourceLanguage() const {
    return ui->translateFrom->currentText();
}

QString MainWindow::getTargetLanguage() const {
    return ui->translateTo->currentText();
}

QString MainWindow::getSourceText() const {
    return ui->textEdit->toPlainText();
}

void MainWindow::setBlurTextEdit(bool status)
{
    gifOverlay->setParent(ui->textEdit->parentWidget());
    if (status)
    {
        auto blur = new QGraphicsBlurEffect(ui->textEdit);

        blur->setBlurRadius(6);
        ui->textEdit->setGraphicsEffect(blur);

        gifOverlay->setAttribute(Qt::WA_TransparentForMouseEvents);
        gifOverlay->setAttribute(Qt::WA_TranslucentBackground);
        gifOverlay->setStyleSheet("background: transparent;");
        gifOverlay->setFixedSize(64, 64);
        gifOverlay->setScaledContents(true);
        gifOverlay->setMovie(loadingSpinner);
        gifOverlay->movie()->start();
        gifOverlay->raise();
        gifOverlay->show();

        ui->textEdit->viewport()->installEventFilter(this);

        positionOverlay();
    } else {
        ui->textEdit->setGraphicsEffect(nullptr);
        gifOverlay->hide();
        applyShadowEffect();
    }
}

void MainWindow::setBlurTextEdit_2(bool status)
{
    gifOverlay->setParent(ui->textEdit_2->parentWidget());
    if (status)
    {
        auto blur = new QGraphicsBlurEffect(ui->textEdit_2);
        blur->setBlurRadius(6);
        ui->textEdit_2->setGraphicsEffect(blur);

        gifOverlay->setAttribute(Qt::WA_TransparentForMouseEvents);
        gifOverlay->setAttribute(Qt::WA_TranslucentBackground);
        gifOverlay->setStyleSheet("background: transparent;");
        gifOverlay->setFixedSize(64, 64);
        gifOverlay->setScaledContents(true);
        gifOverlay->setMovie(loadingSpinner);
        gifOverlay->movie()->start();
        gifOverlay->raise();
        gifOverlay->show();

        ui->textEdit_2->viewport()->installEventFilter(this);

        positionOverlay();
    } else {
        ui->textEdit_2->setGraphicsEffect(nullptr);
        gifOverlay->hide();
        applyShadowEffect();
    }
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

void MainWindow::copyFromTextEdit()
{
    QString text;

    if (sender() == ui->copy_1Button) {
        text = ui->textEdit->toPlainText();
    } else if (sender() == ui->copy_2Button) {
        text = ui->textEdit_2->toPlainText();
    }

    if (text.endsWith('\n'))
        text.chop(1);

    QApplication::clipboard()->setText(text);
}

void MainWindow::collapse()
{
    this->showMinimized();
    isCollapsed = true;
}

void MainWindow::clear()
{
    ui->textEdit->clear();
    ui->textEdit_2->clear();
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->textEdit->viewport() && event->type() == QEvent::Resize) {
        positionOverlay();
    }

    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::positionOverlay()
{
    QWidget* vp = ui->textEdit->viewport();
    QPoint topLeft = vp->mapTo(ui->textEdit->parentWidget(), QPoint(0,0));
    int x = topLeft.x() + (vp->width()  - gifOverlay->width())  / 2;
    int y = topLeft.y() + (vp->height() - gifOverlay->height()) / 2;
    gifOverlay->move(x, y);
}

void MainWindow::handleSwap()
{
    QString origText = ui->textEdit->toPlainText();
    QString transText = ui->textEdit_2->toPlainText();

    if (transText.isEmpty()) return;

    ui->textEdit->setText(transText);
    ui->textEdit_2->clear();

    QString fromLang = ui->translateFrom->currentText();
    QString toLang = ui->translateTo->currentText();

    if (fromLang.startsWith("Auto Detect (") && fromLang.endsWith(")")) {
        fromLang = fromLang.mid(13, fromLang.length() - 14);
    }
    else if (fromLang == "Auto Detect") {
        fromLang = "English";
    }

    ui->translateFrom->setItemText(0, "Auto Detect");

    ui->translateFrom->blockSignals(true);
    ui->translateTo->blockSignals(true);

    int fromIdx = -1;
    for (int i = 0; i < ui->translateFrom->count(); ++i) {
        if (ui->translateFrom->itemText(i).compare(toLang, Qt::CaseInsensitive) == 0) {
            fromIdx = i;
            break;
        }
    }

    int toIdx = -1;
    for (int i = 0; i < ui->translateTo->count(); ++i) {
        if (ui->translateTo->itemText(i).compare(fromLang, Qt::CaseInsensitive) == 0) {
            toIdx = i;
            break;
        }
    }

    if (fromIdx != -1) ui->translateFrom->setCurrentIndex(fromIdx);
    if (toIdx != -1) ui->translateTo->setCurrentIndex(toIdx);

    ui->translateFrom->blockSignals(false);
    ui->translateTo->blockSignals(false);

    if (m_triBtn) {
        m_triBtn->setLeftText(ui->translateFrom->currentText());
        m_triBtn->setRightText(ui->translateTo->currentText());
    }

    emit swapRequested();
}


