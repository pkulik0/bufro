#pragma once

#include <memory>

#include <QApplication>
#include <QSystemTrayIcon>
#include <QMenu>

#include "about_dialog.hxx"
#include "capture.hxx"
#include "recorder.hxx"

class TrayMenu final : public QMenu {
    Q_OBJECT
public:
    explicit TrayMenu(QWidget *parent = nullptr);
private:
    std::unique_ptr<QAction> screenshot_action_{std::make_unique<QAction>("Screenshot", this)};

    std::unique_ptr<QAction> about_action_{std::make_unique<QAction>("About", this)};
    std::unique_ptr<AboutDialog> about_window_{std::make_unique<AboutDialog>(this)};

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


class Application final : public QApplication {
    Q_OBJECT
public:
    Application(int argc, char** argv);
private:
    std::unique_ptr<TrayIcon> trayIcon_{std::make_unique<TrayIcon>()};
    std::unique_ptr<QHotkey> image_hotkey_{std::make_unique<QHotkey>(QKeySequence("Ctrl+Shift+O"), true)};
    std::unique_ptr<QHotkey> video_hotkey_{std::make_unique<QHotkey>(QKeySequence("Ctrl+Shift+E"), true)};
    std::unique_ptr<ScreenRecorder> recorder_{std::make_unique<ScreenRecorder>(this)};
};