#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QColor>
#include <QRandomGenerator>
#include <QVsoa>
#include <QDebug>
#include <QTimer>

constexpr char SERVER_PASSWORD[] = "123456";

int count=0;
int m_lightshowLedIndex = 1; // 新增成员变量，当前闪烁的LED编号
MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
       , ui(new Ui::MainWindow)
       , m_client(nullptr)
       , m_lightshowTimer(new QTimer(this))
   {
       ui->setupUi(this);



       // 设置初始文本
       ui->textDisplay->setText("OLED启动！\n"
   );

       // 初始化灯状态
       initLamps();

       // 设置调节旋钮初始值
       ui->dialAdjust->setValue(50);
       ui->textDisplay->append("调节旋钮初始值：50");

       // 初始化QVsoaClient连接
       initVsoaClient();
       // 灯光秀定时器初始化
       m_lightshowTimer->setInterval(10); // 100ms变色一次，可根据需要调整
       connect(m_lightshowTimer, &QTimer::timeout, this, [this]() {
           if (m_client && m_client->isConnected()) {
               lightshowon(m_client, m_lightshowLedIndex);
               m_lightshowLedIndex++;
               if (m_lightshowLedIndex > 6) m_lightshowLedIndex = 1;
           }
       });
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
    if(count==3){
      ui->textDisplay->clear();
      count=0;
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
    cleartext();
    int brightness = QRandomGenerator::global()->bounded(100);
    ui->textDisplay->append(QString("亮度传感器：%1 lux").arg(brightness));
}

void MainWindow::on_btnUltrasonic_clicked()
{
    cleartext();
    float distance = QRandomGenerator::global()->bounded(500) / 10.0;
    ui->textDisplay->append(QString("超声波测距：%1 cm").arg(distance));
}

void MainWindow::on_btnDht11_clicked()
{
    cleartext();
    float temperature = QRandomGenerator::global()->bounded(10, 40);
    float humidity = QRandomGenerator::global()->bounded(20, 80);
    ui->textDisplay->append(QString("dht11：温度 %1 °C，湿度 %2 %").arg(temperature).arg(humidity));
}

void MainWindow::on_btnBuzzerOn_clicked()
{
    cleartext();
    clearoled(m_client);
    ui->textDisplay->append("清屏");//原来是蜂鸣器开启
}

void MainWindow::on_btnBuzzerOff_clicked()
{
    cleartext();
    ui->textDisplay->append("蜂鸣器：关闭");
}

void MainWindow::on_btnMotorOn_clicked()
{
    cleartext();
    ui->textDisplay->append("电机：开启");
}

void MainWindow::on_btnMotorOff_clicked()
{
    cleartext();
    ui->textDisplay->append("电机：关闭");
}

void MainWindow::on_btnPwmRainbow_clicked()
{
    cleartext();
    if (!m_pwmRainbowOn) {
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
    if (!m_lightshowOn) {
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
    QObject::connect(m_client, &QVsoaClient::connected, std::bind(displaytext, m_client, "HELLO,SYLIXOS!"));

    // 连接到服务器（需要根据实际情况修改服务器地址和端口）
    m_client->connect2server("vsoa://127.0.0.1:5666", SERVER_PASSWORD);//oled
    //m_client->connect2server("vsoa://127.0.0.1:5600", SERVER_PASSWORD);//led_pwm
    //m_client->connect2server("vsoa://127.0.0.1:6600", SERVER_PASSWORD);//led_mono
    // 设置自动重连
    m_client->autoConnect(1000, 500);

    ui->textDisplay->append("正在连接VSOA服务器...");
}

