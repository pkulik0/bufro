#include "auth.hxx"

#include <QtCore/QEventLoop>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>

#include "network.hxx"

Auth & Auth::instance() {
    static Auth instance;
    return instance;
}

auto Auth::login() const -> void {
    oauth2_->grant();
}

auto Auth::logout() -> void {
    oauth2_->setToken(QString());
    oauth2_->setRefreshToken(QString());
    oauth2_->expirationAtChanged(QDateTime());
    clear();
    emit logged_out();
}

auto Auth::get_token() const -> QString {
    if (oauth2_->expirationAt() < QDateTime::currentDateTime()) {
        oauth2_->refreshAccessToken();

        QEventLoop loop;
        connect(oauth2_.get(), &QOAuth2AuthorizationCodeFlow::granted, &loop, &QEventLoop::quit);
        connect(oauth2_.get(), &QOAuth2AuthorizationCodeFlow::error, &loop, &QEventLoop::quit);
        loop.exec();

        qDebug() << "Token refreshed";
        // TODO: handle error
    }
    return oauth2_->token();
}

auto Auth::load() -> void {
    const auto refresh_token = Settings::instance().get("refresh_token").toString();
    if (refresh_token.isEmpty()) return;
    const auto token = Settings::instance().get("token").toString();
    const auto expiration_at = Settings::instance().get("expiration_at").toDateTime();

    qDebug() << "Using existing credentials";
    oauth2_->setToken(token);
    oauth2_->setRefreshToken(refresh_token);
    oauth2_->expirationAtChanged(expiration_at);

    fetch_user_info();
}

auto Auth::get_user_info() const -> UserInfo {
    return user_info_;
}

auto Auth::fetch_user_info() -> void {
    const auto req = QNetworkRequest(QUrl(AUTH_USER_INFO_URL.c_str()));
    Network::instance().get(req, [this](QNetworkReply *reply) {
        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << "Error: " << reply->errorString();
            return;
        }

        const auto data = reply->readAll();
        const auto doc = QJsonDocument::fromJson(data);
        const auto obj = doc.object();

        const auto uid = obj["sub"].toString().toStdString();
        const auto email = obj["email"].toString().toStdString();

        user_info_.id = uid;
        user_info_.email = email;
    }, true);
}

auto Auth::save() const -> void {
    Settings::instance().set("token", oauth2_->token(), false);
    Settings::instance().set("refresh_token", oauth2_->refreshToken(), false);
    Settings::instance().set("expiration_at", oauth2_->expirationAt());
}

auto Auth::clear() -> void {
    Settings::instance().remove("token");
    Settings::instance().remove("refresh_token");
    Settings::instance().remove("expiration_at");
    qDebug() << "Auth cleared";
}

Auth::Auth() : QObject(nullptr) {
    oauth2_->setAuthorizationUrl(QUrl(AUTH_LOGIN_URL.data()));
    oauth2_->setAccessTokenUrl(QUrl(AUTH_TOKEN_URL.data()));
    oauth2_->setClientIdentifier(AUTH_CLIENT_ID.data());
    oauth2_->setClientIdentifierSharedKey(AUTH_CLIENT_SECRET.data());
    oauth2_->setScope("openid profile");
    oauth2_->setReplyHandler(replyHandler_.get());

    // set access type to offline
    oauth2_->setModifyParametersFunction([this](QAbstractOAuth::Stage stage, QMultiMap<QString, QVariant>* parameters) {
        if (stage == QAbstractOAuth::Stage::RequestingAuthorization) {
            parameters->insert("access_type", "offline");
        }
    });

    connect(oauth2_.get(), &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser, &QDesktopServices::openUrl);
    connect(oauth2_.get(), &QOAuth2AuthorizationCodeFlow::granted, [this] {
        fetch_user_info();
        save();
        emit authorized();
    });
    connect(oauth2_.get(), &QOAuth2AuthorizationCodeFlow::tokenChanged, [this] {
        save();
    });
    connect(oauth2_.get(), &QOAuth2AuthorizationCodeFlow::refreshTokenChanged, [this] {
        save();
    });
    connect(oauth2_.get(), &QOAuth2AuthorizationCodeFlow::expirationAtChanged, [this] {
        save();
    });
    connect(oauth2_.get(), &QOAuth2AuthorizationCodeFlow::error, [this](const auto &error) {
        qDebug() << "Auth error: " << error;
        clear();
    });
}
