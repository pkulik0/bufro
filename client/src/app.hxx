#pragma once

#include <memory>

#include <QApplication>
#include <QSystemTrayIcon>
#include <QMenu>

#include "about_dialog.hxx"
#include "capture_widget.hxx"
#include "screen_capturer.hxx"

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
    static auto instance() -> TrayIcon& {
        static TrayIcon instance;
        return instance;
    }

private:
    explicit TrayIcon(QWidget *parent = nullptr);

    std::unique_ptr<TrayMenu> tray_menu_;

    QIcon icon_{QIcon(TRAY_ICON_PATH.data())};
    QIcon pause_icon_{QIcon(PAUSE_ICON_PATH.data())};
    QIcon upload_icon_{QIcon(UPLOAD_ICON_PATH.data())};
};


class Application final : public QApplication {
    Q_OBJECT
public:
    Application(int argc, char** argv);
private:
    TrayIcon& tray_icon_{TrayIcon::instance()};
    std::unique_ptr<QHotkey> image_hotkey_{std::make_unique<QHotkey>(QKeySequence("Ctrl+Shift+O"), true)};
    std::unique_ptr<QHotkey> video_hotkey_{std::make_unique<QHotkey>(QKeySequence("Ctrl+Shift+E"), true)};
};
