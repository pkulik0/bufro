#pragma once

#include <memory>

#include <QApplication>
#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QHotkey>

#include "capture.hxx"

class TrayMenu final : public QMenu {
public:
    explicit TrayMenu(QWidget *parent = nullptr);
};

class TrayIcon final : public QSystemTrayIcon {
public:
    explicit TrayIcon(QWidget *parent = nullptr);
private:
    std::unique_ptr<TrayMenu> tray_menu_;
};

class MainWindow final : public QMainWindow {
public:
    MainWindow();
private:
    std::unique_ptr<TrayIcon> trayIcon_;
};

class Application final : public QApplication {
public:
    Application(int argc, char** argv);
private:
    std::unique_ptr<MainWindow> main_window_;
    std::unique_ptr<CaptureWidget> capture_widget_;
    std::unique_ptr<QHotkey> hotkey_;
};