#pragma once

#include <QHotkey>
#include <QWidget>
#include <QMouseEvent>
#include <QPainter>

class CaptureWidget final : public QWidget {
    Q_OBJECT
public:
    static CaptureWidget& instance() {
        static CaptureWidget instance;
        return instance;
    }

    auto toggle() -> void;
protected:
    auto mousePressEvent(QMouseEvent* event) -> void override;
    auto mouseMoveEvent(QMouseEvent* event) -> void override;
    auto mouseReleaseEvent(QMouseEvent* event) -> void override;
    auto paintEvent(QPaintEvent* event) -> void override;
    auto showEvent(QShowEvent* event) -> void override;
    auto hideEvent(QHideEvent* event) -> void override;
private:
    QPoint start_{};
    QPoint end_{};
    bool is_drawing_{false};

    QBrush fill_brush_{QColor(50, 100, 200, 50)};
    QPen border_pen_{QColor(50, 100, 200, 255), 2};
    QBrush overlay_brush_{QColor(0, 0, 0, 100)};

    std::unique_ptr<QHotkey> quit_hotkey_{std::make_unique<QHotkey>(QKeySequence("Escape"))};

    CaptureWidget();

    auto capture() const -> void;
};