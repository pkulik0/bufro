#pragma once

#include <QDialog>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

#include "constants.hxx"
#include "version.hxx"

class AboutDialog final : public QDialog {
    Q_OBJECT
public:
    explicit AboutDialog(QWidget *parent = nullptr) : QDialog(parent) {
        setWindowTitle("About Bufro");
        setFixedSize(300, 200);
        setWindowIcon(icon_);

        setLayout(main_layout_.get());

        icon_label_->setPixmap(icon_pixmap_);
        icon_label_->setAlignment(Qt::AlignCenter);

        main_layout_->addWidget(icon_label_.get());

        main_layout_->addStretch();

        info_layout_->addWidget(author_label_.get());
        info_layout_->addWidget(version_label_.get());
        info_layout_->addWidget(build_time_label_.get());
        main_layout_->addLayout(info_layout_.get());

        main_layout_->addStretch();

        main_layout_->addWidget(close_button_.get());
        connect(close_button_.get(), &QPushButton::clicked, this, &QWidget::close);
    }
private:
    template <typename T>
    using ptr = std::unique_ptr<T>;

    ptr<QWidget> central_widget_{std::make_unique<QWidget>()};
    ptr<QVBoxLayout> main_layout_{std::make_unique<QVBoxLayout>()};

    QIcon icon_{ICON_PATH.data()};
    QPixmap icon_pixmap_{icon_.pixmap(64, 64)};
    ptr<QLabel> icon_label_{std::make_unique<QLabel>()};

    ptr<QVBoxLayout> info_layout_{std::make_unique<QVBoxLayout>()};
    ptr<QLabel> author_label_{std::make_unique<QLabel>("<b>Author:</b> pkulik")};
    ptr<QLabel> version_label_{std::make_unique<QLabel>("<b>Version:</b> " VERSION)};
    ptr<QLabel> build_time_label_{std::make_unique<QLabel>("<b>Build time:</b> " BUILD_TIME)};

    ptr<QPushButton> close_button_{std::make_unique<QPushButton>("Close")};
};
