#include "app.hxx"

TrayMenu::TrayMenu(QWidget *parent) : QMenu(parent) {
    addAction("Quit", []() { QApplication::quit(); });
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
