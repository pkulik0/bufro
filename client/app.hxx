#pragma once

#include <memory>

#include <QApplication>
#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMenu>

#include "capture.hxx"

class TrayMenu final : public QMenu {
    Q_OBJECT
public:
    explicit TrayMenu(QWidget *parent = nullptr);
private:
    std::unique_ptr<QAction> screenshot_action_{std::make_unique<QAction>("Screenshot", this)};

    std::unique_ptr<QAction> login_action_{std::make_unique<QAction>("Login", this)};
    std::unique_ptr<QAction> logout_action_{std::make_unique<QAction>("Logout", this)};

    std::unique_ptr<QAction> quit_action_{std::make_unique<QAction>("Quit", this)};
};

class TrayIcon final : public QSystemTrayIcon {
    Q_OBJECT
public:
    explicit TrayIcon(QWidget *parent = nullptr);
private:
    std::unique_ptr<TrayMenu> tray_menu_;
};

class MainWindow final : public QMainWindow {
    Q_OBJECT
public:
    MainWindow();
private:
    std::unique_ptr<TrayIcon> trayIcon_;
};

class Application final : public QApplication {
    Q_OBJECT
public:
    Application(int argc, char** argv);
private:
    std::unique_ptr<MainWindow> main_window_;
    std::unique_ptr<QHotkey> hotkey_;
};