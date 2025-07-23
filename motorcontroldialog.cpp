#include "motorcontroldialog.h"
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

MotorControlDialog::MotorControlDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("电机控制");
    setFixedSize(320, 180);

    btnStart = new QPushButton("启动", this);
    btnStop = new QPushButton("停止", this);
    btnClose = new QPushButton("关闭", this);

    sliderSpeed = new QSlider(Qt::Horizontal, this);
    sliderSpeed->setRange(0, 5000);
    sliderSpeed->setValue(1200);
    sliderSpeed->setTickInterval(100);
    sliderSpeed->setTickPosition(QSlider::TicksBelow);

    labelSpeedValue = new QLabel(QString::number(sliderSpeed->value()), this);
    labelUnit = new QLabel("rpm", this);

    QHBoxLayout* speedLayout = new QHBoxLayout;
    speedLayout->addWidget(new QLabel("转速:", this));
    speedLayout->addWidget(sliderSpeed);
    speedLayout->addWidget(labelSpeedValue);
    speedLayout->addWidget(labelUnit);

    QHBoxLayout* btnLayout = new QHBoxLayout;
    btnLayout->addWidget(btnStart);
    btnLayout->addWidget(btnStop);
    btnLayout->addWidget(btnClose);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(speedLayout);
    mainLayout->addSpacing(10);
    mainLayout->addLayout(btnLayout);
    setLayout(mainLayout);

    connect(btnStart, &QPushButton::clicked, this, &MotorControlDialog::onStartClicked);
    connect(btnStop, &QPushButton::clicked, this, &MotorControlDialog::onStopClicked);
    connect(btnClose, &QPushButton::clicked, this, &MotorControlDialog::onCloseClicked);
    connect(sliderSpeed, &QSlider::valueChanged, this, &MotorControlDialog::onSetSpeedChanged);
}

MotorControlDialog::~MotorControlDialog() {}

void MotorControlDialog::onStartClicked() {
    emit startMotor();
}

void MotorControlDialog::onStopClicked() {
    emit stopMotor();
}

void MotorControlDialog::onSetSpeedChanged(int value) {
    labelSpeedValue->setText(QString::number(value));
    emit setSpeed(value);
}

void MotorControlDialog::onCloseClicked() {
    this->close();
} 