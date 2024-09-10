/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   02_asr_demo
* @brief         mainwindow.h
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @net           www.openedv.com
* @date          2021-06-04
*******************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QLabel>
#include <QMovie>
#include <QTimer>
#include <QHBoxLayout>
#include "../audiorecorder/audiorecorder.h"
#include "../asr/asr.h"
#include "../led/led.h"
#include "../camera/camera.h"

#include <QTextBrowser>
#include <QTextEdit>
#include <QPushButton>


class Asr;
class AudioRecorder;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



private:

    /* 主Widget */
    QWidget *mainWidget;

    /* gif底下的Widget */
    QWidget *movieWidget;

    /* gif动画画布 */
    QLabel *movieLabel;

    /* 用于显示识别结果 */
    QLabel *textLabel;



    /* 垂直布局 */
    QVBoxLayout *vBoxLayout1;
    QVBoxLayout *vBoxLayout2;
    QVBoxLayout *vBoxLayout3;
    /* 水平布局 */
    QHBoxLayout *hBoxLayout;

    /* 用于显示GIF动画 */
    QMovie *myMovie;
    /* 聊天框 */
    QVBoxLayout* chatLayout;
    QTextBrowser* chatBrowser;
    QTextEdit* chatInput;
    QPushButton* sendButton;
    /* 摄像头设备 */
    Camera *camera;
    /* 用于显示视频的标签 */
    QLabel *videoLabel;
    QLabel *V_textLabel;

    /* 定时器 */
    QTimer *timer1;
    QTimer *timer2;
    QTimer *timer3;

    /* 事件过滤器 */
    bool eventFilter(QObject *watched, QEvent *event);

    /* 录音类 */
    AudioRecorder *myAudioRecorder;

    /* 主意识别类 */
    Asr *myAsr;

    /* 开发板LED */
    Led *myLed;


private slots:
    void onTimer1TimeOut();
    void onTimer2TimeOut();
    void onTimer3TimeOut();
    void onAsrReadyData(QString);
    void sendMessage();
    void onStateChanged();

};
#endif // MAINWINDOW_H
