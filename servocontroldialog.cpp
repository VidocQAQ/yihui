#include "servocontroldialog.h"
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

ServoControlDialog::ServoControlDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("舵机控制");
    setFixedSize(320, 180);

    btnStart = new QPushButton("启动", this);
    btnClose = new QPushButton("关闭", this);
    btnDirection = new QPushButton("正转", this);

    sliderAngle = new QSlider(Qt::Horizontal, this);
    sliderAngle->setRange(0, 360);
    sliderAngle->setValue(90);
    sliderAngle->setTickInterval(10);
    sliderAngle->setTickPosition(QSlider::TicksBelow);
    sliderAngle->setEnabled(false);

    labelAngleValue = new QLabel(QString::number(sliderAngle->value()), this);
    labelUnit = new QLabel("°", this);

    QHBoxLayout* angleLayout = new QHBoxLayout;
    angleLayout->addWidget(new QLabel("角度:", this));
    angleLayout->addWidget(sliderAngle);
    angleLayout->addWidget(labelAngleValue);
    angleLayout->addWidget(labelUnit);

    QHBoxLayout* btnLayout = new QHBoxLayout;
    btnLayout->addWidget(btnStart);
    btnLayout->addWidget(btnDirection);
    btnLayout->addWidget(btnClose);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(angleLayout);
    mainLayout->addSpacing(10);
    mainLayout->addLayout(btnLayout);
    setLayout(mainLayout);

    connect(btnStart, &QPushButton::clicked, this, &ServoControlDialog::onStartClicked);
    connect(btnClose, &QPushButton::clicked, this, &ServoControlDialog::onCloseClicked);
    connect(btnDirection, &QPushButton::clicked, this, &ServoControlDialog::onDirectionClicked);
    connect(sliderAngle, &QSlider::valueChanged, this, &ServoControlDialog::onSetAngleChanged);
}

ServoControlDialog::~ServoControlDialog() {}

void ServoControlDialog::onStartClicked() {
    isRunning = !isRunning;
    if (isRunning) {
        btnStart->setText("停止");
        btnStart->setStyleSheet("background-color:#38b000;color:white;");
        sliderAngle->setEnabled(true);
        emit startServo();
    } else {
        btnStart->setText("启动");
        btnStart->setStyleSheet("");
        sliderAngle->setEnabled(false);
        emit stopServo();
    }
}

void ServoControlDialog::onDirectionClicked() {
    isForward = !isForward;
    btnDirection->setText(isForward ? "正转" : "反转");
    emit setAngle(sliderAngle->value()); // 可根据需要发信号
}

void ServoControlDialog::onSetAngleChanged(int value) {
    labelAngleValue->setText(QString::number(value));
    emit setAngle(value);
}

void ServoControlDialog::onCloseClicked() {
    this->close();
} 