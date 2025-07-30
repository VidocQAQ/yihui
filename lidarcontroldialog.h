#pragma once
#include <QDialog>
#include <QPushButton>
#include "lidarpolarwidget.h"
#include <QTimer>

class QVsoaClient; // 前向声明

class LidarControlDialog : public QDialog {
    Q_OBJECT
public:
    explicit LidarControlDialog(QVsoaClient* client, QWidget* parent = nullptr);
    ~LidarControlDialog();
    void setLidarPoints();

private slots:
    void onStartClicked();
    void onCloseClicked();

private:
    QVsoaClient* m_client;
    QPushButton* btnStart;
    QPushButton* btnClose;
    LidarPolarWidget* polarWidget;
    QTimer* testTimer = nullptr;
    bool isRunning = false;
    void generateTestData();
}; 
