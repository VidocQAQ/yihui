#ifndef SERVOCONTROLDIALOG_H
#define SERVOCONTROLDIALOG_H

#include <QDialog>

class QPushButton;
class QSlider;
class QLabel;

class ServoControlDialog : public QDialog {
    Q_OBJECT
public:
    explicit ServoControlDialog(QWidget* parent = nullptr);
    ~ServoControlDialog();

signals:
    void startServo();
    void stopServo();
    void setAngle(int angle);

private slots:
    void onStartClicked();
    void onSetAngleChanged(int value);
    void onCloseClicked();
    void onDirectionClicked();

private:
    QPushButton* btnStart;
    QPushButton* btnClose;
    QPushButton* btnDirection;
    QSlider* sliderAngle;
    QLabel* labelAngleValue;
    QLabel* labelUnit;
    bool isRunning = false;
    bool isForward = true;
};

#endif // SERVOCONTROLDIALOG_H 