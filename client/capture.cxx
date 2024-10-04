#include "capture.hxx"

#include <QPainter>
#include <QTimer>
#include <QApplication>

CaptureWidget::CaptureWidget(QWidget *parent) : QWidget(parent), is_drawing_(false) {
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setCursor(Qt::CrossCursor);
    show();
    activateWindow();
}

auto CaptureWidget::mousePressEvent(QMouseEvent *event) -> void {
    if (event->button() != Qt::LeftButton) return;
    start_ = event->pos();
    end_ = event->pos();
    is_drawing_ = true;
}

auto CaptureWidget::mouseMoveEvent(QMouseEvent *event) -> void {
    if (!is_drawing_) return;
    end_ = event->pos();
    update();
}

auto CaptureWidget::mouseReleaseEvent(QMouseEvent *event) -> void {
    if (event->button() != Qt::LeftButton) return;
    end_ = event->pos();
    is_drawing_ = false;
    update();
}

auto CaptureWidget::paintEvent(QPaintEvent *event) -> void {
    QPainter painter(this);

    // Draw the background
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(QColor(0, 0, 0, 50)));
    painter.drawRect(rect());

    if (!is_drawing_) return;

    // Draw the selection rectangle
    painter.setPen(QPen(Qt::red, 2));
    painter.setBrush(QBrush(QColor(255, 0, 0, 50)));
    painter.drawRect(QRect(start_, end_));
}

auto CaptureWidget::showEvent(QShowEvent *event) -> void {
    QWidget::showEvent(event);

    // Set the widget geometry to cover all screens
    QRect totalScreenGeometry;
    for (const auto* screen : QApplication::screens()) {
        totalScreenGeometry = totalScreenGeometry.united(screen->geometry());
    }
    setGeometry(totalScreenGeometry);
}