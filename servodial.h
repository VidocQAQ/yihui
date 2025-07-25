#pragma once
#include <QWidget>

class ServoDial : public QWidget {
    Q_OBJECT
public:
    explicit ServoDial(QWidget* parent = nullptr);
    void setRange(int min, int max);
    void setAngle(int angle);
    int angle() const;

signals:
    void angleChanged(int angle);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    int m_min = 0;
    int m_max = 180;
    int m_angle = 90;
    QPoint m_center;
    int m_radius = 60;
    void updateAngleFromPos(const QPoint& pos);
}; 