#pragma once
#include <QtWidgets/qwidget.h>
#include <QMouseEvent>

class CaptureWidget final : public QWidget {
public:
    explicit CaptureWidget(QWidget* parent = nullptr);
protected:
    auto mousePressEvent(QMouseEvent* event) -> void override;
    auto mouseMoveEvent(QMouseEvent* event) -> void override;
    auto mouseReleaseEvent(QMouseEvent* event) -> void override;
    auto paintEvent(QPaintEvent* event) -> void override;
    auto showEvent(QShowEvent* event) -> void override;
private:
    QPoint start_;
    QPoint end_;
    bool is_drawing_;
};