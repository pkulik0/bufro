#pragma once

#include <QDir>
#include <QMediaRecorder>
#include <QMediaFormat>
#include <QUrl>
#include <QScreenCapture>
#include <QMediaCaptureSession>
#include <QTimer>

class ScreenCapturer final : public QObject {
    Q_OBJECT
public:
    static auto instance() -> ScreenCapturer& {
        static ScreenCapturer instance;
        return instance;
    }

    auto start_recording(QScreen* screen, QRect rect) -> void;

    auto stop_recording() -> void;

    static auto screenshot(QScreen* screen, QRect rect) -> void;

signals:
    auto recording_started() -> void;
    auto recording_stopped() -> void;
    auto processing_finished() -> void;
private:
    explicit ScreenCapturer(QObject* parent = nullptr);

    template<typename T>
    using ptr = std::unique_ptr<T>;

    enum class State {
        IDLE, RECORDING, PROCESSING
    };
    State state_{State::IDLE};

    QRect rect_{0, 0, 700, 700};
    QString video_path_{QDir::tempPath() + "/bufro_screen.mp4"};
    QString cropped_path_{QDir::tempPath() + "/bufro_cropped.mp4"};

    ptr<QScreenCapture> screen_capture_{std::make_unique<QScreenCapture>(this)};
    ptr<QMediaRecorder> recorder_{std::make_unique<QMediaRecorder>(this)};
    ptr<QMediaCaptureSession> session_{std::make_unique<QMediaCaptureSession>(this)};
    QMediaFormat format_{};

    QTimer timer_{};

    auto crop() -> void;
    auto upload() -> void;
    auto cleanup() -> void;
};
