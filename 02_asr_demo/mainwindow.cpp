/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   02_asr_demo
* @brief         mainwindow.cpp
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @net           www.openedv.com
* @date          2021-06-04
*******************************************************************/
#include "mainwindow.h"
#include <QDebug>
#include <QSound>
#include <unistd.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setGeometry(0, 0, 800, 480);
//    this->setStyleSheet("background:#14161a");

    mainWidget = new QWidget();
    movieWidget = new QWidget();

    movieLabel = new QLabel();
    textLabel = new QLabel();

    // 创建视频标签
    videoLabel = new QLabel();
    V_textLabel = new QLabel();
    V_textLabel->setMinimumSize(240, 50);
    V_textLabel->setStyleSheet("color: yellow; font-size: 20px");
    V_textLabel->setText("表情识别");
    V_textLabel->setAlignment(Qt::AlignCenter);

    movieWidget->setMinimumWidth(240);
    textLabel->setMinimumSize(240, 50);
    textLabel->setStyleSheet("color: yellow; font-size: 20px");
    textLabel->setText("请点击，开始说话...");
    textLabel->setAlignment(Qt::AlignCenter);

    // 初始化聊天控件
    chatBrowser = new QTextBrowser();
    chatInput = new QTextEdit();
    sendButton = new QPushButton("发送");

    chatLayout = new QVBoxLayout();
    chatLayout->addWidget(chatBrowser);
    chatLayout->addWidget(chatInput);
    chatLayout->addWidget(sendButton);


    //设置视频标签
    videoLabel = new QLabel;
    videoLabel->setFixedSize(240,240);
    videoLabel->setStyleSheet("background-color: black;");
    // 连接发送按钮的点击信号到槽函数
    connect(sendButton, &QPushButton::clicked, this, &MainWindow::sendMessage);
    /* 安装事件过滤器 */
    movieLabel->installEventFilter(this);
    movieLabel->setFixedSize(240, 240);

    vBoxLayout1 = new QVBoxLayout();
    vBoxLayout2 = new QVBoxLayout();
    vBoxLayout3 = new QVBoxLayout();
    hBoxLayout = new QHBoxLayout();

    vBoxLayout1->addWidget(movieLabel);
    vBoxLayout1->addWidget(textLabel);
    vBoxLayout1->setAlignment(Qt::AlignCenter);

    vBoxLayout2->addWidget(videoLabel);
    vBoxLayout2->addWidget(V_textLabel);
    vBoxLayout2->setAlignment(Qt::AlignCenter);
    // 将聊天布局添加到主布局
    hBoxLayout->addLayout(vBoxLayout1);
    hBoxLayout->addLayout(vBoxLayout2);
    vBoxLayout3->addLayout(hBoxLayout);
    vBoxLayout3->addLayout(chatLayout);

    mainWidget->setLayout(vBoxLayout3);
//    hBoxLayout->addWidget(movieLabel);
//    movieWidget->setLayout(hBoxLayout);

    setCentralWidget(mainWidget);

    myMovie = new QMovie(":/gif/voice_effect.gif");
    /* 设置播放速度，值越大越快 */
    myMovie->setSpeed(50);
    movieLabel->setMovie(myMovie);
    movieLabel->show();
    /* 设置设置化时显示第一帧 */
    myMovie->jumpToNextFrame();

    movieLabel->setAlignment(Qt::AlignHCenter);

    timer1 = new QTimer(this);
    timer2 = new QTimer(this);
    timer3 = new QTimer(this);

    connect(timer1, SIGNAL(timeout()), this, SLOT(onTimer1TimeOut()));
    connect(timer2, SIGNAL(timeout()), this, SLOT(onTimer2TimeOut()));
    connect(timer3, SIGNAL(timeout()), this, SLOT(onTimer3TimeOut()));

    /* 自定义的录音类 */
    myAudioRecorder = new AudioRecorder(this);

    /* 自定义的百度云识别录音类 */
    myAsr = new Asr(this);

    /* LED */
    myLed = new Led(this);

    //  录音状态发生变化时会发送信号stateChanged
    connect(myAudioRecorder, &AudioRecorder::recordingStopped,
            this, &MainWindow::onStateChanged);


    connect(myAsr, SIGNAL(asrReadyData(QString)), this, SLOT(onAsrReadyData(QString)));
}

MainWindow::~MainWindow()
{
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event){

    if (watched == movieLabel && event->type() == QEvent::MouseButtonPress) {
        QSound::play(":/audio/sound.wav");
        if (myMovie->state() != QMovie::Running) {
            /* 等待QSound播放完,1.5s后再录音 */
            timer2->start(100);
            textLabel->setText("正在听您说话，请继续...");
            myMovie->start();
        }
    }

    return QWidget::eventFilter(watched, event);
}

void MainWindow::onStateChanged()
{

           // 录音已经停止
           qDebug() << "Audio recording has stopped.";
           textLabel->setText("正在识别，请稍候...");
           timer1->stop();
           myMovie->stop();
           QString fileName = QCoreApplication::applicationDirPath() + "/16k.wav";
           myAsr->getTheResult(fileName);
           timer3->start(30000);

}
void MainWindow::onTimer1TimeOut()
{
    /* 停止录音，8s钟的短语音 */
    myAudioRecorder->stopRecorder();
    textLabel->setText("正在识别，请稍候...");
    timer1->stop();
    myMovie->stop();
    QString fileName = QCoreApplication::applicationDirPath() + "/16k.wav";
    myAsr->getTheResult(fileName);
    timer3->start(30000);
}

void MainWindow::onTimer2TimeOut()
{
    timer1->start(4000);
    /* 开始录音 */
    myAudioRecorder->startRecorder();
    timer2->stop();
}

void MainWindow::onTimer3TimeOut()
{
    textLabel->setText("请点击，开始说话...");
    timer3->stop();
}

void MainWindow::onAsrReadyData(QString str)
{
    if (str.contains("开灯"))
        myLed->setLedState(true);
    else if (str.contains("关灯"))
        myLed->setLedState(false);
    chatBrowser->append(str);
    textLabel->setText("识别结果是:\n" + str);
    textLabel->adjustSize();
}

void MainWindow::sendMessage()
{
    QString message = chatInput->toPlainText();
    if (!message.isEmpty()) {
        // 在聊天窗口中显示自己的消息
        chatBrowser->append("我: " + message);

        // 清空输入框
        chatInput->clear();

        // 模拟接收来自对方的消息，可以根据实际需求替换为真实接收逻辑
        QString response = "对方的回复";
        chatBrowser->append("对方: " + response);
    }
}


