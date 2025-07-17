#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QColor>
#include <QRandomGenerator>
#include <QVsoa>
#include <QDebug>
#include <QTimer>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
// 假设 QVsoaClient/QVsoaPayload 头文件已包含

constexpr char SERVER_PASSWORD[] = "123456";
const QString DHT11_URL = "/sensor/dht11/data";
const QString USS_URL = "/sensor/uss/data";

int count=0;
int count1=0;
int m_lightshowLedIndex = 1; // 新增成员变量，当前闪烁的LED编号
MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
       , ui(new Ui::MainWindow)
       , m_client(nullptr)
       , m_lightshowTimer(new QTimer(this))
   {
       ui->setupUi(this);



       // 设置初始文本
       QString now = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
       ui->textDisplay->setText(QString("HELLO,BAOZI!\n"));
       ui->textDisplay_2->setText(QString("当前时间: %1\n").arg(now));
       // 初始化灯状态
       initLamps();

       // 初始化QVsoaClient连接
       initVsoaClient();
       // 灯光秀定时器初始化
       m_lightshowTimer->setInterval(50); // 300ms变色一次，更慢更赏心悦目
       connect(m_lightshowTimer, &QTimer::timeout, this, [this]() {
           if (m_client && m_client->isConnected()) {
               lightshowon(m_client);
           }
       });
       // 新增：初始化电位器同步定时器
       m_potSyncTimer = new QTimer(this);
       m_potSyncTimer->setInterval(0); // 1秒
       connect(m_potSyncTimer, &QTimer::timeout, this, &MainWindow::syncDialWithPotValue);
       m_potSyncTimer->start();
   }

MainWindow::~MainWindow()
{
    if (m_lightshowTimer) {
        m_lightshowTimer->stop();
        delete m_lightshowTimer;
    }
    if (m_client) {
        m_client->disconnect();
        delete m_client;
    }
    delete ui;
}

void MainWindow::initLamps()
{
    // 设置灯的初始状态
    QList<QLabel*> lamps = {
        ui->lampRed,
        ui->lampYellow,
        ui->lampGreen,
        ui->lampBlue
    };

    QList<QColor> colors = {
        Qt::red,
        Qt::yellow,
        Qt::green,
        Qt::blue
    };

    for (int i = 0; i < lamps.size(); i++) {
        QLabel *lamp = lamps[i];
        lamp->setProperty("on", false);
        lamp->setProperty("originalColor", colors[i].name());
        lamp->setStyleSheet("background-color: #808080; border-radius: 15px;");
    }
}

void MainWindow::toggleLamp(QLabel *lamp)
{
    bool isOn = lamp->property("on").toBool();
    QString originalColor = lamp->property("originalColor").toString();

    if (isOn) {
        lamp->setStyleSheet("background-color: gray; border-radius: 15px;");
    } else {
        lamp->setStyleSheet(QString("background-color: %1; border-radius: 15px;").arg(originalColor));
    }

    lamp->setProperty("on", !isOn);
}

void MainWindow::cleartext(){
    count++;
    if(count==2){
      ui->textDisplay->clear();
      count=0;
    }
}
void MainWindow::cleartext1(){
    count1++;
    if(count1==2){
      ui->textDisplay_2->clear();
      count1=0;
    }
}

void MainWindow::on_btnRed_clicked()
{
    cleartext();
    if (!isRedOn) {
        redmonoon(m_client); // 传入 m_client
        isRedOn = true;
        ui->textDisplay->append("红灯已打开");
        toggleLamp(ui->lampRed);
    } else {
        redmonooff(m_client); // 传入 m_client
        isRedOn = false;
        ui->textDisplay->append("红灯已关闭");
        toggleLamp(ui->lampRed);
    }
}

void MainWindow::on_btnYellow_clicked()
{
    cleartext();
    if (!isYellowOn) {
        yellowmonoon(m_client);
        isYellowOn = true;
        ui->textDisplay->append("黄灯已打开");
        toggleLamp(ui->lampYellow);
    } else {
        yellowmonooff(m_client);
        isYellowOn = false;
        ui->textDisplay->append("黄灯已关闭");
        toggleLamp(ui->lampYellow);
    }
}

void MainWindow::on_btnGreen_clicked()
{
    cleartext();
    if (!isGreenOn) {
        greenmonoon(m_client);
        isGreenOn = true;
        ui->textDisplay->append("绿灯已打开");
        toggleLamp(ui->lampGreen);
    } else {
        greenmonooff(m_client);
        isGreenOn = false;
        ui->textDisplay->append("绿灯已关闭");
        toggleLamp(ui->lampGreen);
    }
}

void MainWindow::on_btnBlue_clicked()
{
    cleartext();
    if (!isBlueOn) {
        bluemonoon(m_client);
        isBlueOn = true;
        ui->textDisplay->append("蓝灯已打开");
        toggleLamp(ui->lampBlue);
    } else {
        bluemonooff(m_client);
        isBlueOn = false;
        ui->textDisplay->append("蓝灯已关闭");
        toggleLamp(ui->lampBlue);
    }
}

void MainWindow::on_btnBrightnessSensor_clicked()
{
    cleartext1();
    if (m_client && m_client->isConnected()) {
        if (!isADCOn) {
            ADCon(m_client);
            ui->textDisplay_2->append("亮度传感器：开启");
            isADCOn = true;
        } else {
            ADCoff(m_client);
            ui->textDisplay_2->append("亮度传感器：关闭");
            isADCOn = false;
        }
    } else {
        ui->textDisplay->append("错误：VSOA客户端未连接，无法发送命令");
    }
}

void MainWindow::on_btnUltrasonic_clicked()
{
    cleartext1();
    if (m_client && m_client->isConnected()) {
        if (!isUSSOn) {
            USSon(m_client);
            ui->textDisplay_2->append("超声波测距：开启");
            isUSSOn = true;
        } else {
            USSoff(m_client);
            ui->textDisplay_2->append("超声波测距：关闭");
            isUSSOn = false;
        }
    } else {
        ui->textDisplay->append("错误：VSOA客户端未连接，无法发送命令");
    }
}

void MainWindow::on_btnDht11_clicked()
{
    cleartext1();
    if (m_client && m_client->isConnected()) {
        if (!isDHT11On) {
            DHT11on(m_client);
            ui->textDisplay_2->append("温湿度传感器：开启");
             isDHT11On = true;
        } else {
            DHT11off(m_client);
            ui->textDisplay_2->append("温湿度传感器：关闭");
            isDHT11On = false;
        }
    } else {
        ui->textDisplay->append("错误：VSOA客户端未连接，无法发送命令");
    }
    
}

void MainWindow::on_btnBuzzerOn_clicked()
{
    cleartext1();
    if (!isBuzzerOn) {
        buzzeron(m_client);
        isBuzzerOn = true;
        ui->textDisplay_2->append("蜂鸣器已开启");
    } else {
        buzzeroff(m_client);
        isBuzzerOn = false;
        ui->textDisplay_2->append("蜂鸣器已关闭");
    }
}

void MainWindow::on_btnBuzzerSongon_clicked()
{
    cleartext1();
    if (!isBuzzerSongOn) {
        buzzersongon(m_client);
        isBuzzerSongOn = true;
        ui->textDisplay_2->append("蜂鸣器唱歌已开始");
    } else {
        buzzersongoff(m_client);
        isBuzzerSongOn = false;
        ui->textDisplay_2->append("蜂鸣器唱歌已停止");
    }
}


void MainWindow::on_btnMotorOn_clicked()
{
    cleartext1();
    if (!isBreathOn) {
        // 启动呼吸灯时，先改变UI上四个灯的颜色
        if (!isRedOn) {
            toggleLamp(ui->lampRed);
            isRedOn = true;
        }
        if (!isYellowOn) {
            toggleLamp(ui->lampYellow);
            isYellowOn = true;
        }
        if (!isGreenOn) {
            toggleLamp(ui->lampGreen);
            isGreenOn = true;
        }
        if (!isBlueOn) {
            toggleLamp(ui->lampBlue);
            isBlueOn = true;
        }
        // 启动呼吸灯
        if (!m_breathTimer) {
            m_breathTimer = new QTimer(this);
            connect(m_breathTimer, &QTimer::timeout, this, [this]() {
                if (!m_client || !m_client->isConnected()) return;
                // 调整亮度
                if (m_breathIncreasing) {
                    m_breathBrightness += 5;
                    if (m_breathBrightness >= 255) {
                        m_breathBrightness = 255;
                        m_breathIncreasing = false;
                    }
                } else {
                    m_breathBrightness -= 5;
                    if (m_breathBrightness <= 0) {
                        m_breathBrightness = 0;
                        m_breathIncreasing = true;
                    }
                }
                breathon(m_client, m_breathBrightness);
            });
        }
        m_breathBrightness = 0;
        m_breathIncreasing = true;
        m_breathTimer->start(30); // 30ms调整一次亮度
        isBreathOn = true;
        ui->textDisplay_2->append("呼吸灯已开启");
    }
}

void MainWindow::on_btnMotorOff_clicked()
{
    cleartext1();
    // 关闭呼吸灯时，先改变UI上四个灯的颜色
    if (isRedOn) {
        toggleLamp(ui->lampRed);
        isRedOn = false;
        redmonooff(m_client);
    }
    if (isYellowOn) {
        toggleLamp(ui->lampYellow);
        isYellowOn = false;
        yellowmonooff(m_client);
    }
    if (isGreenOn) {
        toggleLamp(ui->lampGreen);
        isGreenOn = false;
        greenmonooff(m_client);
    }
    if (isBlueOn) {
        toggleLamp(ui->lampBlue);
        isBlueOn = false;
        bluemonooff(m_client);
    }
    // 关闭呼吸灯
    if (m_breathTimer) m_breathTimer->stop();
    breathoff(m_client);
    isBreathOn = false;
    ui->textDisplay_2->append("呼吸灯已关闭");
}

void MainWindow::on_btnPwmRainbow_clicked()
{
    cleartext();
    m_lightshowTimer->stop();
    lightshowoff(m_client);
    if (!m_pwmRainbowOn) {
        m_lightshowOn=false;
        ui->textDisplay->append("多色灯：rainbow模式启动");
        // 检查客户端是否已连接
        if (m_client && m_client->isConnected()) {
            rainbowon(m_client);
        } else {
            ui->textDisplay->append("错误：VSOA客户端未连接，无法发送命令");
        }
        m_pwmRainbowOn = true;
    } else {
        ui->textDisplay->append("多色灯：rainbow模式关闭");
        // 关闭rainbow模式的逻辑
        if (m_client && m_client->isConnected()) {
            rainbowoff(m_client);
        } else {
            ui->textDisplay->append("错误：VSOA客户端未连接，无法发送关灯命令");
        }
        m_pwmRainbowOn = false;
    }
}

void MainWindow::on_btnPwmLightshow_clicked()
{
    cleartext();
    if(isled_pwmOn(m_client)==1){
        rainbowoff(m_client);
    }
    if (!m_lightshowOn) {
        m_pwmRainbowOn=false;
        ui->textDisplay->append("多色灯：lightshow模式启动");
        if (m_client && m_client->isConnected()) {
            m_lightshowLedIndex = 1; // 启动时从1号灯开始
            m_lightshowTimer->start();
            ui->textDisplay->append("已开始灯光秀");
        } else {
            ui->textDisplay->append("错误：VSOA客户端未连接，无法启动灯光秀");
        }
        m_lightshowOn = true;
    } else {
        ui->textDisplay->append("多色灯：lightshow模式关闭");
        m_lightshowTimer->stop();
        if (m_client && m_client->isConnected()) {
            lightshowoff(m_client);
            ui->textDisplay->append("已发送关灯命令到服务器");
        }
        m_lightshowOn = false;
    }
}

void MainWindow::on_dialAdjust_valueChanged(int value)
{
    cleartext();
    ui->textDisplay->append(QString("调节旋钮：%1").arg(value));
}

void MainWindow::on_textDisplay_copyAvailable(bool b)
{

}


void MainWindow::on_dialAdjust_actionTriggered(int action)
{

}

void MainWindow::initVsoaClient()
{
    // 创建QVsoaClient实例
    m_client = new QVsoaClient(this);

    // 连接信号槽
    QObject::connect(m_client, &QVsoaClient::connected, std::bind(onConnected, std::placeholders::_1, std::placeholders::_2));
    QObject::connect(m_client, &QVsoaClient::disconnected, onDisconnected);
    QObject::connect(m_client, &QVsoaClient::connected, std::bind(displaytext, m_client, "HELLO,BAOZI!"));
    QObject::connect(m_client, &QVsoaClient::message, this,
        [=](QString url, QVsoaPayload payload){
            onMessage(m_client, url, payload);displaySensorStatus();
        });
    

    // 连接到服务器（需要根据实际情况修改服务器地址和端口）
    //m_client->connect2server("vsoa://127.0.0.1:5600", SERVER_PASSWORD);
    m_client->connect2server("vsoa://127.0.0.1:5600", SERVER_PASSWORD);//测试

    // 设置自动重连
    m_client->autoConnect(1000, 500);

    ui->textDisplay->append("正在连接VSOA服务器...");
    m_client->subscribe("/sensor/dht11/data");
    m_client->subscribe("/sensor/uss/data");
    m_client->subscribe("/adc/data");
    m_client->autoConsistent({"/sensor/dht11/data", "/sensor/uss/data", "/adc/data"}, 1000);

}

void MainWindow::displaySensorStatus()
{
     // 根据全局标志输出对应信息
     if (hasDHT11 && !hasUSS && !hasADC) {
        ui->textDisplay_2->append(latestDHT11);
    } else if (hasUSS && !hasDHT11 && !hasADC) {
        ui->textDisplay_2->append(latestUSS);
    } else if (hasADC && !hasDHT11 && !hasUSS) {
        ui->textDisplay_2->append(latestADC);
    } else if (hasDHT11 && hasUSS && !hasADC) {
        QString combined = QString("%1\n%2").arg(latestDHT11).arg(latestUSS);
        ui->textDisplay_2->append(combined);
    } else if (hasDHT11 && hasADC && !hasUSS) {
        QString combined = QString("%1\n%2").arg(latestDHT11).arg(latestADC);
        ui->textDisplay_2->append(combined);
    } else if (hasUSS && hasADC && !hasDHT11) {
        QString combined = QString("%1\n%2").arg(latestUSS).arg(latestADC);
        ui->textDisplay_2->append(combined);
    } else if (hasDHT11 && hasUSS && hasADC) {
        QString combined = QString("%1\n%2\n%3").arg(latestDHT11).arg(latestUSS).arg(latestADC);
        ui->textDisplay_2->append(combined);
    }
    else {
        ui->textDisplay_2->append("未订阅任何传感器数据");
    }
}

void MainWindow::syncDialWithPotValue()
{
    extern int latestPotValueC;
    if (ui->dialAdjust->value() != latestPotValueC) {
        ui->dialAdjust->setValue(latestPotValueC);
    }
}





