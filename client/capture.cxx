#include "capture.hxx"

#include <QPainter>
#include <QApplication>
#include <QTimer>
#include <QBuffer>
#include <QByteArray>

CaptureWidget::CaptureWidget(QWidget *parent) : QWidget(parent), is_drawing_(false), quit_hotkey_(std::make_unique<QHotkey>(QKeySequence("Escape"))) {
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::ToolTip);
    setAttribute(Qt::WA_TranslucentBackground);
    setCursor(Qt::CrossCursor);
    setFocusPolicy(Qt::StrongFocus);

    connect(quit_hotkey_.get(), &QHotkey::activated, this, &QWidget::hide);
}

auto CaptureWidget::hideEvent(QHideEvent *event) -> void {
    quit_hotkey_->setRegistered(false);
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

auto CaptureWidget::capture() const -> void {
    auto* screen = QApplication::screenAt(start_);
    const auto rect = QRect(start_, end_).normalized();
    const auto pixmap = screen->grabWindow(0, rect.x(), rect.y(), rect.width(), rect.height());

    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, "PNG");
    qDebug() << "Captured" << bytes.size() << "bytes";
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

    QRect geom;
    for (const auto* screen : QApplication::screens()) {
        geom = geom.united(screen->geometry());
    }
    setGeometry(geom);

    quit_hotkey_->setRegistered(true);
}

auto CaptureWidget::toggle() -> void {
    if (isVisible()) {
        hide();
    } else {
        show();
    }
}
