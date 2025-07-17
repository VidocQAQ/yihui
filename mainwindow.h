#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QVsoa>
#include "client_rpc.h"
#include "client_subpub.h"
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void displaySensorStatus();
private slots:
    void cleartext1();
    void cleartext();
    void on_btnRed_clicked();
    void on_btnYellow_clicked();
    void on_btnGreen_clicked();
    void on_btnBlue_clicked();
    void on_btnBrightnessSensor_clicked();
    void on_btnUltrasonic_clicked();
    void on_btnDht11_clicked();
    void on_btnBuzzerOn_clicked();
    void on_btnMotorOn_clicked();
    void on_btnMotorOff_clicked();
    void on_btnPwmRainbow_clicked();
    void on_btnPwmLightshow_clicked();
    void on_dialAdjust_valueChanged(int value);

    void on_textDisplay_copyAvailable(bool b);

    void on_dialAdjust_actionTriggered(int action);

    void on_btnBuzzerSongon_clicked();


private:
    Ui::MainWindow *ui;
    QVsoaClient *m_client; // 添加QVsoaClient成员变量
    bool m_pwmRainbowOn = false; // 彩虹灯开关状态
    QTimer *m_lightshowTimer = nullptr; // 灯光秀定时器
    bool m_lightshowOn = false; // 灯光秀开关状态
    bool isRedOn = false;
    bool isYellowOn = false;
    bool isGreenOn = false;
    bool isBlueOn = false;
    bool isBuzzerOn = false; // 蜂鸣器开关状态
    bool isBuzzerSongOn = false; // 蜂鸣器唱歌开关状态
    bool isDHT11On = true;
    bool isUSSOn = true;
    bool isADCOn = true;
    bool isBreathOn = false; // 呼吸灯开关状态
    QTimer *m_breathTimer = nullptr; // 呼吸灯定时器
    int m_breathBrightness = 0; // 当前呼吸灯亮度
    bool m_breathIncreasing = true; // 呼吸灯亮度变化方向
    // 发布/订阅客户端
    QVsoaClient *vsoaClient = nullptr;
    QTimer *m_potSyncTimer = nullptr;
    void syncDialWithPotValue();

    // 初始化灯的状态
    void initLamps();

    // 切换灯状态
    void toggleLamp(QLabel *lamp);
    
    // 初始化QVsoaClient连接
    void initVsoaClient();
};

#endif // MAINWINDOW_H
