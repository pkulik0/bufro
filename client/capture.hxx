#pragma once

#include <QHotkey>
#include <QWidget>
#include <QMouseEvent>

class CaptureWidget final : public QWidget {
public:
    explicit CaptureWidget(QWidget* parent = nullptr);
    auto toggle() -> void;
protected:
    auto mousePressEvent(QMouseEvent* event) -> void override;
    auto mouseMoveEvent(QMouseEvent* event) -> void override;
    auto mouseReleaseEvent(QMouseEvent* event) -> void override;
    auto paintEvent(QPaintEvent* event) -> void override;
    auto showEvent(QShowEvent* event) -> void override;
    auto hideEvent(QHideEvent* event) -> void override;
private:
    QPoint start_;
    QPoint end_;
    bool is_drawing_;

    std::unique_ptr<QHotkey> quit_hotkey_;
    auto capture() const -> void;
};