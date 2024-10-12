#include "screen_capturer.hxx"

#include <QApplication>
#include <QBuffer>
#include <QDir>
#include <QTimer>

#include <opencv2/opencv.hpp>

#include "network.hxx"

namespace fs = std::filesystem;

auto ScreenCapturer::crop() -> void {
    qDebug() << "Cropping captured video to:" << rect_;

    cv::VideoCapture cap(video_path_.toStdString());
    if (!cap.isOpened()) {
        qDebug() << "Error opening video stream or file";
        return;
    }

    const auto fps = cap.get(cv::CAP_PROP_FPS);
    const auto codec = static_cast<int>(cap.get(cv::CAP_PROP_FOURCC));

    cv::VideoWriter writer(cropped_path_.toStdString(), codec, fps, cv::Size(rect_.width(), rect_.height()));
    if (!writer.isOpened()) {
        qDebug() << "Error opening video writer";
        cap.release();
        return;
    }

    cv::Mat frame;
    while(cap.read(frame)) {
        if(rect_.x() + rect_.width() > frame.cols || rect_.y() + rect_.height() > frame.rows) {
            qDebug() << "Invalid rect" << rect_;
            writer.release();
            cap.release();
            return;
        }

        const cv::Rect roi(rect_.x(), rect_.y(), rect_.width(), rect_.height());
        const cv::Mat cropped = frame(roi);
        writer.write(cropped);
    }

    qDebug() << "Finished processing" << cropped_path_;
    writer.release();
    cap.release();

    upload();
}

auto ScreenCapturer::upload() -> void {
    qDebug() << "Uploading" << cropped_path_;

    QFile file(cropped_path_);
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open" << cropped_path_;
        cleanup();
        return;
    }

    QByteArray bytes = file.readAll();
    api::upload(BUF_TYPE_VIDEO, bytes);

    cleanup();
}

auto ScreenCapturer::cleanup() -> void {
    if(!fs::remove(video_path_.toStdString())) {
        qDebug() << "Failed to remove" << video_path_;
    }
    if(!fs::remove(cropped_path_.toStdString())) {
        qDebug() << "Failed to remove" << cropped_path_;
    }

    emit processing_finished();
    state_ = State::IDLE;
}

ScreenCapturer::ScreenCapturer(QObject *parent): QObject(parent) {
    connect(recorder_.get(), &QMediaRecorder::errorOccurred, [](QMediaRecorder::Error error, const QString &errorString) {
        qDebug() << "Recorder error" << error << errorString;
    });
    connect(screen_capture_.get(), &QScreenCapture::errorOccurred, [](QScreenCapture::Error error, const QString &errorString) {
        qDebug() << "Screen capture error" << error << errorString;
    });

    connect(&timer_, &QTimer::timeout, this, &ScreenCapturer::stop_recording);
    timer_.setSingleShot(true);
    timer_.setInterval(15000);;

    format_.setVideoCodec(QMediaFormat::VideoCodec::H265);
    format_.setAudioCodec(QMediaFormat::AudioCodec::AAC);
    format_.setFileFormat(QMediaFormat::FileFormat::MPEG4);

    recorder_->setMediaFormat(format_);
    recorder_->setVideoFrameRate(30);
    recorder_->setVideoBitRate(6 * 1000 * 1000);
    recorder_->setQuality(QMediaRecorder::VeryHighQuality);

    session_->setRecorder(recorder_.get());
    screen_capture_->setScreen(QApplication::primaryScreen());
    session_->setScreenCapture(screen_capture_.get());
}

auto ScreenCapturer::start_recording(QScreen* screen, QRect rect) -> void {
    if(state_ != State::IDLE) {
        qDebug() << "Already recording";
        return;
    }
    state_ = State::RECORDING;

    const auto dpr = screen->devicePixelRatio();
    const auto x = rect.x() * dpr;
    const auto y = rect.y() * dpr;
    const auto width = rect.width() * dpr;
    const auto height = rect.height() * dpr;
    rect_ = QRect(static_cast<int>(x), static_cast<int>(y), static_cast<int>(width), static_cast<int>(height));
    screen_capture_->setScreen(screen);

    qDebug() << "Starting recording" << video_path_;
    recorder_->setOutputLocation(QUrl::fromLocalFile(video_path_));

    screen_capture_->start();
    recorder_->record();

    emit recording_started();
    timer_.start();
}

auto ScreenCapturer::stop_recording() -> void {
    if(state_ != State::RECORDING) {
        qDebug() << "Not recording";
        return;
    }

    qDebug() << "Stopping recording";

    recorder_->stop();
    screen_capture_->stop();

    state_ = State::PROCESSING;
    emit recording_stopped();
    timer_.stop();

    QTimer::singleShot(500, this, &ScreenCapturer::crop);
}

auto ScreenCapturer::screenshot(QScreen *screen, QRect rect) -> void {
    const auto pixmap = screen->grabWindow(0, rect.x(), rect.y(), rect.width(), rect.height());
    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, "WEBP");

    api::upload(BUF_TYPE_IMAGE, bytes);
}
