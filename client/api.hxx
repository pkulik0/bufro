#pragma once

#include <memory>
#include <QDesktopServices>

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QOAuth2AuthorizationCodeFlow>
#include <QOAuthHttpServerReplyHandler>

class QAbstractOAuthReplyHandler;
class QOAuth2AuthorizationCodeFlow;

class Network final : public QObject {
public:
    static Network& instance() {
        static Network instance;
        return instance;
    }

    auto get(QNetworkRequest& req, const auto on_reply, const bool authorized = false) -> void {
        const auto token = oauth2_->token();
        if (authorized && token.isEmpty()) return; // TODO: err
        req.setRawHeader("Authorization", "Bearer " + token.toUtf8());

        auto *reply = network_manager_->get(req);
        connect(reply, &QNetworkReply::finished, [reply, on_reply] {
            on_reply(reply);
            reply->deleteLater();
        });
    }

    template<typename T>
    auto post(QNetworkRequest req, const T& data, const auto on_reply, const bool authorized = false) -> void {
        const auto token = oauth2_->token();
        if (authorized && token.isEmpty()) return; // TODO: err
        req.setRawHeader("Authorization", "Bearer " + token.toUtf8());

        auto *reply = network_manager_->post(req, data);
        connect(reply, &QNetworkReply::finished, [reply, on_reply] {
            on_reply(reply);
            reply->deleteLater();
        });
    }

    auto login() const -> void {
        oauth2_->grant();
    }

private:
    std::unique_ptr<QNetworkAccessManager> network_manager_{std::make_unique<QNetworkAccessManager>(nullptr)};
    std::unique_ptr<QOAuth2AuthorizationCodeFlow> oauth2_{std::make_unique<QOAuth2AuthorizationCodeFlow>(this)};
    std::unique_ptr<QOAuthHttpServerReplyHandler> replyHandler_{std::make_unique<QOAuthHttpServerReplyHandler>(this)};

    Network() {
        oauth2_->setAuthorizationUrl(QUrl("http://localhost:8081/realms/bufro/protocol/openid-connect/auth"));
        oauth2_->setAccessTokenUrl(QUrl("http://localhost:8081/realms/bufro/protocol/openid-connect/token"));
        oauth2_->setClientIdentifier("app");
        oauth2_->setClientIdentifierSharedKey("YITlLPjL8kpXynn6qIPOaEWIXwHlHavo");
        oauth2_->setScope("openid profile");
        oauth2_->setReplyHandler(replyHandler_.get());

        connect(oauth2_.get(), &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser, &QDesktopServices::openUrl);
        connect(oauth2_.get(), &QOAuth2AuthorizationCodeFlow::granted, [] { qDebug() << "Granted"; });
        connect(oauth2_.get(), &QOAuth2AuthorizationCodeFlow::tokenChanged, [] { qDebug() << "Token changed"; });
    }
};
