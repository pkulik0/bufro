#pragma once

#include <memory>

#include <QDesktopServices>
#include <QOAuth2AuthorizationCodeFlow>
#include <QOAuthHttpServerReplyHandler>
#include <utility>

#include "settings.hxx"
#include "constants.hxx"

class Auth final : public QObject {
    Q_OBJECT

    struct UserInfo {
        std::string id{};
        std::string email{};

        UserInfo() = default;
        UserInfo(std::string id, std::string email) : id(std::move(id)), email(std::move(email)) {}

        auto operator=(const UserInfo& other) -> UserInfo& = default;
    };

    UserInfo user_info_{};

    Auth();

    auto save() const -> void;

    static auto clear() -> void;

    auto fetch_user_info() -> void;

    std::unique_ptr<QOAuth2AuthorizationCodeFlow> oauth2_{std::make_unique<QOAuth2AuthorizationCodeFlow>(this)};
    std::unique_ptr<QOAuthHttpServerReplyHandler> replyHandler_{std::make_unique<QOAuthHttpServerReplyHandler>(this)};
public:
    static Auth& instance();

    auto login() const -> void ;

    auto logout() -> void ;

    [[nodiscard]] auto get_token() const -> QString;

    auto load() -> void;

    [[nodiscard]] auto get_user_info() const -> UserInfo;

signals:
    auto authorized() -> void;
    auto logged_out() -> void;
};