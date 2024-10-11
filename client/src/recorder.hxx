#pragma once

#include <QDir>
#include <QMediaRecorder>
#include <QMediaFormat>
#include <QUrl>
#include <QScreenCapture>
#include <QMediaCaptureSession>

class ScreenRecorder final : public QObject {
    Q_OBJECT
public:
    explicit ScreenRecorder(QObject* parent = nullptr);

    auto start() -> void ;

    auto stop() -> void ;

private:
    template<typename T>
    using ptr = std::unique_ptr<T>;

    bool is_recording_{false};

    QRect rect_{0, 0, 700, 700};
    QString video_path_{QDir::tempPath() + "/bufro_screen.mp4"};
    QString cropped_path_{QDir::tempPath() + "/bufro_cropped.mp4"};

    ptr<QScreenCapture> screen_capture_{std::make_unique<QScreenCapture>(this)};
    ptr<QMediaRecorder> recorder_{std::make_unique<QMediaRecorder>(this)};
    ptr<QMediaCaptureSession> session_{std::make_unique<QMediaCaptureSession>(this)};
    QMediaFormat format_{};

    auto process() -> void;
    auto cleanup() -> void;
};
