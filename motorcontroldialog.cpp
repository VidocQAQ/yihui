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
    setFixedSize(420, 260);

    btnStart = new QPushButton("启动", this);
    btnClose = new QPushButton("关闭", this);
    btnDirection = new QPushButton("正转", this);
    btnSpeed1 = new QPushButton("500", this);
    btnSpeed2 = new QPushButton("1200", this);
    btnSpeed3 = new QPushButton("2500", this);
    btnSpeed4 = new QPushButton("4000", this);
    btnCustom = new QPushButton("自定义", this);
    btnCustom->setEnabled(false);

    sliderSpeed = new QSlider(Qt::Horizontal, this);
    sliderSpeed->setRange(0, 5000);
    sliderSpeed->setValue(1200);
    sliderSpeed->setTickInterval(100);
    sliderSpeed->setTickPosition(QSlider::TicksBelow);
    sliderSpeed->setEnabled(false);

    labelSpeedValue = new QLabel(QString::number(sliderSpeed->value()), this);
    labelUnit = new QLabel("rpm", this);

    // 档位按钮布局
    QHBoxLayout* presetLayout = new QHBoxLayout;
    presetLayout->addWidget(new QLabel("档位:", this));
    presetLayout->addWidget(btnSpeed1);
    presetLayout->addWidget(btnSpeed2);
    presetLayout->addWidget(btnSpeed3);
    presetLayout->addWidget(btnSpeed4);
    presetLayout->addWidget(btnCustom);

    // 速度调节布局
    QHBoxLayout* speedLayout = new QHBoxLayout;
    speedLayout->addWidget(new QLabel("转速:", this));
    speedLayout->addWidget(sliderSpeed);
    speedLayout->addWidget(labelSpeedValue);
    speedLayout->addWidget(labelUnit);

    // 按钮布局
    QHBoxLayout* btnLayout = new QHBoxLayout;
    btnLayout->addWidget(btnStart);
    btnLayout->addWidget(btnDirection);
    btnLayout->addWidget(btnClose);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(presetLayout);
    mainLayout->addLayout(speedLayout);
    mainLayout->addSpacing(10);
    mainLayout->addLayout(btnLayout);
    setLayout(mainLayout);

    connect(btnStart, &QPushButton::clicked, this, &MotorControlDialog::onStartClicked);
    connect(btnClose, &QPushButton::clicked, this, &MotorControlDialog::onCloseClicked);
    connect(btnDirection, &QPushButton::clicked, this, &MotorControlDialog::onDirectionClicked);
    connect(sliderSpeed, &QSlider::valueChanged, this, &MotorControlDialog::onSetSpeedChanged);
    connect(btnSpeed1, &QPushButton::clicked, this, &MotorControlDialog::onSpeedPresetClicked);
    connect(btnSpeed2, &QPushButton::clicked, this, &MotorControlDialog::onSpeedPresetClicked);
    connect(btnSpeed3, &QPushButton::clicked, this, &MotorControlDialog::onSpeedPresetClicked);
    connect(btnSpeed4, &QPushButton::clicked, this, &MotorControlDialog::onSpeedPresetClicked);

    // 默认禁用档位按钮
    btnSpeed1->setEnabled(false);
    btnSpeed2->setEnabled(false);
    btnSpeed3->setEnabled(false);
    btnSpeed4->setEnabled(false);
    // 自定义按钮初始原色
    btnCustom->setStyleSheet("");
}

MotorControlDialog::~MotorControlDialog() {}

void MotorControlDialog::onStartClicked() {
    isRunning = !isRunning;
    if (isRunning) {
        btnStart->setText("停止");
        btnStart->setStyleSheet("background-color:#38b000;color:white;");
        // 启动时自动切换到最低档位
        sliderSpeed->setValue(speedPresets[0]);
        labelSpeedValue->setText(QString::number(speedPresets[0]));
        currentPresetIndex = 0;
        btnSpeed1->setStyleSheet("background-color:#4cc9f0;color:white;");
        btnSpeed2->setStyleSheet("");
        btnSpeed3->setStyleSheet("");
        btnSpeed4->setStyleSheet("");
        btnCustom->setStyleSheet("");
        // 启用档位按钮和转速条
        btnSpeed1->setEnabled(true);
        btnSpeed2->setEnabled(true);
        btnSpeed3->setEnabled(true);
        btnSpeed4->setEnabled(true);
        sliderSpeed->setEnabled(true);
        emit startMotor();
    } else {
        btnStart->setText("启动");
        btnStart->setStyleSheet("");
        // 停止时转速条和数值显示为0，档位按钮全部取消高亮
        sliderSpeed->setValue(0);
        labelSpeedValue->setText("0");
        currentPresetIndex = -1;
        btnSpeed1->setStyleSheet("");
        btnSpeed2->setStyleSheet("");
        btnSpeed3->setStyleSheet("");
        btnSpeed4->setStyleSheet("");
        btnCustom->setStyleSheet("");
        // 禁用档位按钮和转速条
        btnSpeed1->setEnabled(false);
        btnSpeed2->setEnabled(false);
        btnSpeed3->setEnabled(false);
        btnSpeed4->setEnabled(false);
        sliderSpeed->setEnabled(false);
        emit stopMotor();
    }
}

void MotorControlDialog::onSetSpeedChanged(int value) {
    labelSpeedValue->setText(QString::number(value));
    emit setSpeed(value);
    // 拖动条变化时取消档位高亮
    currentPresetIndex = -1;
    btnSpeed1->setStyleSheet("");
    btnSpeed2->setStyleSheet("");
    btnSpeed3->setStyleSheet("");
    btnSpeed4->setStyleSheet("");
    // 拖动条变化高亮自定义按钮
    btnCustom->setStyleSheet("background-color:#f72585;color:white;");
}

void MotorControlDialog::onCloseClicked() {
    this->close();
}

void MotorControlDialog::onDirectionClicked() {
    isForward = !isForward;
    btnDirection->setText(isForward ? "正转" : "反转");
    emit setDirection(isForward);
}

void MotorControlDialog::onSpeedPresetClicked() {
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    int idx = -1;
    if (btn == btnSpeed1) idx = 0;
    else if (btn == btnSpeed2) idx = 1;
    else if (btn == btnSpeed3) idx = 2;
    else if (btn == btnSpeed4) idx = 3;
    if (idx >= 0) {
        sliderSpeed->setValue(speedPresets[idx]);
        currentPresetIndex = idx;
        // 高亮当前档位
        btnSpeed1->setStyleSheet(idx==0 ? "background-color:#4cc9f0;color:white;" : "");
        btnSpeed2->setStyleSheet(idx==1 ? "background-color:#4cc9f0;color:white;" : "");
        btnSpeed3->setStyleSheet(idx==2 ? "background-color:#4cc9f0;color:white;" : "");
        btnSpeed4->setStyleSheet(idx==3 ? "background-color:#4cc9f0;color:white;" : "");
        // 恢复自定义按钮原色
        btnCustom->setStyleSheet("");
    }
} 