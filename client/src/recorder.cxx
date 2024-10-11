#include "recorder.hxx"

#include <QApplication>
#include <QDesktopServices>
#include <QDir>
#include <QTimer>

#include <opencv2/opencv.hpp>

namespace fs = std::filesystem;

auto ScreenRecorder::process() -> void {
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

    qDebug() << "Finished processing";
    writer.release();
    cap.release();

    cleanup();
}

auto ScreenRecorder::cleanup() -> void {
    if(!fs::remove(video_path_.toStdString())) {
        qDebug() << "Failed to remove" << video_path_;
    }
    if(!fs::remove(cropped_path_.toStdString())) {
        qDebug() << "Failed to remove" << cropped_path_;
    }
    is_recording_ = false;
}

ScreenRecorder::ScreenRecorder(QObject *parent): QObject(parent) {
    connect(recorder_.get(), &QMediaRecorder::errorOccurred, [](QMediaRecorder::Error error, const QString &errorString) {
        qDebug() << "Recorder error" << error << errorString;
    });
    connect(screen_capture_.get(), &QScreenCapture::errorOccurred, [](QScreenCapture::Error error, const QString &errorString) {
        qDebug() << "Screen capture error" << error << errorString;
    });

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

auto ScreenRecorder::start() -> void {
    if(is_recording_) {
        qDebug() << "Already recording";
        return;
    }
    is_recording_ = true;

    qDebug() << "Starting recording" << video_path_;
    recorder_->setOutputLocation(QUrl::fromLocalFile(video_path_));

    screen_capture_->start();
    recorder_->record();

    QTimer::singleShot(5000, this, &ScreenRecorder::stop);
}

auto ScreenRecorder::stop() -> void {
    qDebug() << "Stopping recording";

    recorder_->stop();
    screen_capture_->stop();

    QTimer::singleShot(500, this, &ScreenRecorder::process);
}
