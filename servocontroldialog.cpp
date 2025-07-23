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
    btnStop = new QPushButton("停止", this);
    btnClose = new QPushButton("关闭", this);

    sliderAngle = new QSlider(Qt::Horizontal, this);
    sliderAngle->setRange(0, 360);
    sliderAngle->setValue(90);
    sliderAngle->setTickInterval(10);
    sliderAngle->setTickPosition(QSlider::TicksBelow);

    labelAngleValue = new QLabel(QString::number(sliderAngle->value()), this);
    labelUnit = new QLabel("°", this);

    QHBoxLayout* angleLayout = new QHBoxLayout;
    angleLayout->addWidget(new QLabel("角度:", this));
    angleLayout->addWidget(sliderAngle);
    angleLayout->addWidget(labelAngleValue);
    angleLayout->addWidget(labelUnit);

    QHBoxLayout* btnLayout = new QHBoxLayout;
    btnLayout->addWidget(btnStart);
    btnLayout->addWidget(btnStop);
    btnLayout->addWidget(btnClose);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(angleLayout);
    mainLayout->addSpacing(10);
    mainLayout->addLayout(btnLayout);
    setLayout(mainLayout);

    connect(btnStart, &QPushButton::clicked, this, &ServoControlDialog::onStartClicked);
    connect(btnStop, &QPushButton::clicked, this, &ServoControlDialog::onStopClicked);
    connect(btnClose, &QPushButton::clicked, this, &ServoControlDialog::onCloseClicked);
    connect(sliderAngle, &QSlider::valueChanged, this, &ServoControlDialog::onSetAngleChanged);
}

ServoControlDialog::~ServoControlDialog() {}

void ServoControlDialog::onStartClicked() {
    emit startServo();
}

void ServoControlDialog::onStopClicked() {
    emit stopServo();
}

void ServoControlDialog::onSetAngleChanged(int value) {
    labelAngleValue->setText(QString::number(value));
    emit setAngle(value);
}

void ServoControlDialog::onCloseClicked() {
    this->close();
} 