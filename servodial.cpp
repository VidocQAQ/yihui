#include "servodial.h"
#include <QPainter>
#include <QMouseEvent>
#include <QtMath>

ServoDial::ServoDial(QWidget* parent) : QWidget(parent) {
    setMinimumSize(100, 100);
}

void ServoDial::setRange(int min, int max) {
    m_min = min;
    m_max = max;
    update();
}

void ServoDial::setAngle(int angle) {
    if (angle < m_min) angle = m_min;
    if (angle > m_max) angle = m_max;
    if (m_angle != angle) {
        m_angle = angle;
        emit angleChanged(m_angle);
        update();
    }
}

int ServoDial::angle() const {
    return m_angle;
}

void ServoDial::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    int w = width();
    int h = height();
    m_center = QPoint(w/2, h-10); // 半圆中心在下方
    m_radius = qMin(w, 2*h)/2 - 10;
    // 画半圆盘
    p.setPen(Qt::NoPen);
    p.setBrush(QColor(230,230,230));
    QRectF rect(m_center.x()-m_radius, m_center.y()-m_radius, 2*m_radius, 2*m_radius);
    p.drawPie(rect, 0*16, 180*16); // 画下半圆
    // 画刻度
    p.setPen(QPen(Qt::gray, 2));
    for (int a = m_min; a <= m_max; a += 10) {
        double rad = M_PI - a*M_PI/(m_max-m_min); // 0°在左，180°在右
        QPointF p1 = m_center + QPointF(qCos(rad)*(m_radius-8), -qSin(rad)*(m_radius-8));
        QPointF p2 = m_center + QPointF(qCos(rad)*(m_radius), -qSin(rad)*(m_radius));
        p.drawLine(p1, p2);
    }
    // 画指针
    double rad = M_PI - m_angle*M_PI/(m_max-m_min);
    QPointF p1 = m_center;
    QPointF p2 = m_center + QPointF(qCos(rad)*(m_radius-16), -qSin(rad)*(m_radius-16));
    p.setPen(QPen(Qt::red, 4));
    p.drawLine(p1, p2);
    // 画当前角度数字
    p.setPen(Qt::black);
    QFont f = p.font();
    f.setPointSize(10);
    p.setFont(f);
    p.drawText(QRect(m_center.x()-m_radius, m_center.y()-m_radius-30, 2*m_radius, 30), Qt::AlignCenter, QString::number(m_angle) + QChar(176));
}

void ServoDial::mousePressEvent(QMouseEvent* event) {
    updateAngleFromPos(event->pos());
}

void ServoDial::mouseMoveEvent(QMouseEvent* event) {
    if (event->buttons() & Qt::LeftButton) {
        updateAngleFromPos(event->pos());
    }
}

void ServoDial::resizeEvent(QResizeEvent*) {
    update();
}

void ServoDial::updateAngleFromPos(const QPoint& pos) {
    double dx = pos.x() - m_center.x();
    double dy = m_center.y() - pos.y();
    double angle = qAtan2(dy, dx) * 180.0 / M_PI;
    // 只响应下半圆
    if (angle < 0) angle += 360;
    if (angle > 180) return;
    // 修正方向：向右为角度增大
    int newAngle = m_min + (180 - angle) * (m_max - m_min) / 180.0;
    setAngle(newAngle);
} 