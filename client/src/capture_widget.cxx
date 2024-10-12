#include "capture_widget.hxx"

#include <QPainter>
#include <QApplication>

#include "pb/bufro.pb.h"
#include "screen_capturer.hxx"

CaptureWidget::CaptureWidget() : QWidget(nullptr) {
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::ToolTip);
    setAttribute(Qt::WA_TranslucentBackground);
    setCursor(Qt::CrossCursor);
    setFocusPolicy(Qt::StrongFocus);

    connect(quit_hotkey_.get(), &QHotkey::activated, this, &CaptureWidget::close);
}

auto CaptureWidget::hideEvent(QHideEvent *event) -> void {
    quit_hotkey_->setRegistered(false);
    is_drawing_ = false;
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

    hide();
    QTimer::singleShot(200, this, &CaptureWidget::capture);
}

auto CaptureWidget::capture() -> void {
    auto* screen = QApplication::screenAt(start_);
    const auto rect = QRect(start_, end_).normalized();

    switch(type_) {
        case Type::IMAGE: {
            ScreenCapturer::screenshot(screen, rect);
            break;
        }
        case Type::VIDEO: {
            ScreenCapturer::instance().start_recording(screen, rect);
            break;
        }
        default: {
            qDebug() << "Unknown capture type";
            break;
        }
    }
    type_ = Type::UNKNOWN;
}

auto CaptureWidget::close() -> void {
    type_ = Type::UNKNOWN;
    hide();
}

auto CaptureWidget::paintEvent(QPaintEvent *event) -> void {
    QPainter painter(this);

    // Draw the background
    painter.setPen(Qt::NoPen);
    painter.setBrush(overlay_brush_);
    painter.drawRect(rect());

    if (!is_drawing_) return;

    // Draw the selection rectangle
    painter.setPen(border_pen_);
    painter.setBrush(fill_brush_);
    painter.drawRect(QRect(start_, end_));
}

auto CaptureWidget::showEvent(QShowEvent *event) -> void {
    QWidget::showEvent(event);

    QRect geom;
    for (const auto* screen : QApplication::screens()) {
        geom = geom.united(screen->geometry());
    }
    setGeometry(geom);
    setFixedSize(geom.size());

    quit_hotkey_->setRegistered(true);
}

auto CaptureWidget::toggle(Type type) -> void {
    if(type_ != Type::UNKNOWN && type_ != type) {
        qDebug() << "Already capturing";
        return;
    }

    if (isVisible()) {
        qDebug() << "Hiding capture";
        close();
    } else {
        qDebug() << "Showing capture";
        show();
        type_ = type;
    }
}
