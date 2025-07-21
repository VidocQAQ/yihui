#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QVsoa>
#include "client_rpc.h"
#include "client_subpub.h"
#include <QTimer>
#include <QPropertyAnimation>

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
    void removeSensorValue(const QString& key);
    void updateSensorValue(const QString& key, const QString& value);
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

    // LED状态同步定时器
    QTimer *m_ledStatusTimer = nullptr;
    
    // 呼吸灯UI特效定时器
    QTimer *m_breathUITimer = nullptr;
    int m_breathUIAlpha = 255; // UI呼吸灯透明度
    bool m_breathUIIncreasing = true; // UI呼吸灯透明度变化方向
    
    // 初始化灯的状态
    void initLamps();

    // 切换灯状态
    void toggleLamp(QLabel *lamp);
    
    // 更新LED状态显示
    void updateLedStatusDisplay();
    void updateLedStatusDisplay(const QJsonObject &status);
    
    // 同步LED状态
    void syncLedStatus();
    
    // 呼吸灯UI特效
    void startBreathUIEffect();
    void stopBreathUIEffect();
    void updateBreathUIEffect();
    
    // 测试LED状态查询
    void testLedStatusQuery();
    
    // 初始化QVsoaClient连接
    void initVsoaClient();
};

#endif // MAINWINDOW_H
