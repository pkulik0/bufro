#include "app.hxx"

#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>

#include "network.hxx"

TrayMenu::TrayMenu(QWidget *parent) : QMenu(parent) {
    connect(quit_action_.get(), &QAction::triggered, [] { qApp->quit(); });
    connect(login_action_.get(), &QAction::triggered, [] { Auth::instance().login(); });
    connect(logout_action_.get(), &QAction::triggered, [] { Auth::instance().logout(); });
    connect(&Auth::instance(), &Auth::authorized, [this] {
        removeAction(login_action_.get());
        insertAction(quit_action_.get(), logout_action_.get());
        insertSeparator(quit_action_.get());
    });
    connect(&Auth::instance(), &Auth::logged_out, [this] {
        removeAction(logout_action_.get());
        insertAction(quit_action_.get(), login_action_.get());
        insertSeparator(quit_action_.get());
    });

    addAction(login_action_.get());
    addSeparator();
    addAction(quit_action_.get());

    Auth::instance().load();
}

TrayIcon::TrayIcon(QWidget *parent) : QSystemTrayIcon(parent), tray_menu_(std::make_unique<TrayMenu>()) {
    setIcon(QIcon("/Users/pkulik/repos/bufro/client/assets/icon.png"));  // TODO: include icon in build
    setToolTip("Bufro");
    setContextMenu(tray_menu_.get());
    show();
}

MainWindow::MainWindow() : QMainWindow(nullptr), trayIcon_(std::make_unique<TrayIcon>(this)) {

}

Application::Application(int argc, char** argv)
    : QApplication(argc, argv), main_window_(std::make_unique<MainWindow>()), capture_widget_(std::make_unique<CaptureWidget>()),
hotkey_(std::make_unique<QHotkey>(QKeySequence("Ctrl+Shift+O"), true)) {
    connect(hotkey_.get(), &QHotkey::activated, capture_widget_.get(), &CaptureWidget::toggle);
}
