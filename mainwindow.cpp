#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QColor>
#include <QRandomGenerator>
#include <QVsoa>
#include <QDebug>

constexpr char SERVER_PASSWORD[] = "123456";

int count=0;

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
    , m_client(nullptr)
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
}

MainWindow::~MainWindow()
{
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
    toggleLamp(ui->lampRed);
    ui->textDisplay->append("红灯状态切换");
}

void MainWindow::on_btnYellow_clicked()
{
    cleartext();
    toggleLamp(ui->lampYellow);
    ui->textDisplay->append("黄灯状态切换");
}

void MainWindow::on_btnGreen_clicked()
{
    cleartext();
    toggleLamp(ui->lampGreen);
    ui->textDisplay->append("绿灯状态切换");
}

void MainWindow::on_btnBlue_clicked()
{
    cleartext();
    toggleLamp(ui->lampBlue);
    ui->textDisplay->append("蓝灯状态切换");
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
    ui->textDisplay->append("蜂鸣器：开启");
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
            // 调用lightCall函数
            lightCall(m_client);
            ui->textDisplay->append("已发送rainbow模式命令到服务器");
        } else {
            ui->textDisplay->append("错误：VSOA客户端未连接，无法发送命令");
        }
        m_pwmRainbowOn = true;
    } else {
        ui->textDisplay->append("多色灯：rainbow模式关闭");
        // 关闭rainbow模式的逻辑
        if (m_client && m_client->isConnected()) {
            lightOffCall(m_client);
            ui->textDisplay->append("已发送关灯命令到服务器");
        } else {
            ui->textDisplay->append("错误：VSOA客户端未连接，无法发送关灯命令");
        }
        m_pwmRainbowOn = false;
    }
}

void MainWindow::on_btnPwmLightshow_clicked()
{
    cleartext();
    ui->textDisplay->append("多色灯：lightshow模式启动");
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
    
    // 连接到服务器（需要根据实际情况修改服务器地址和端口）
    m_client->connect2server("vsoa://127.0.0.1:5600", SERVER_PASSWORD);
    // 设置自动重连
    m_client->autoConnect(1000, 500);
    
    ui->textDisplay->append("正在连接VSOA服务器...");
}

