#ifndef MOTORCONTROLDIALOG_H
#define MOTORCONTROLDIALOG_H

#include <QDialog>

class QPushButton;
class QSlider;
class QLabel;

class MotorControlDialog : public QDialog {
    Q_OBJECT
public:
    explicit MotorControlDialog(QWidget* parent = nullptr);
    ~MotorControlDialog();

signals:
    void startMotor();
    void stopMotor();
    void setSpeed(int speed);

private slots:
    void onStartClicked();
    void onStopClicked();
    void onSetSpeedChanged(int value);
    void onCloseClicked();

private:
    QPushButton* btnStart;
    QPushButton* btnStop;
    QPushButton* btnClose;
    QSlider* sliderSpeed;
    QLabel* labelSpeedValue;
    QLabel* labelUnit;
};

#endif // MOTORCONTROLDIALOG_H 