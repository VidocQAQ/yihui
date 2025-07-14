#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QColor>
#include <QRandomGenerator>

int count=0;

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
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
}

MainWindow::~MainWindow()
{
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
    if(count==1){
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
    ui->textDisplay->append("多色灯：rainbow模式启动");
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

