#pragma once

#include <memory>

#include <QNetworkRequest>
#include <QNetworkReply>

#include "auth.hxx"

class Network final : public QObject {
    Q_OBJECT
public:
    static Network& instance() {
        static Network instance;
        return instance;
    }

    auto get(QNetworkRequest req, const auto on_reply, const bool authorized = false) -> void {
        const auto token = Auth::instance().get_token();
        if (authorized && token.isEmpty()) {
            Auth::instance().login();
            return; // TODO: err?
        };
        req.setRawHeader("Authorization", "Bearer " + token.toUtf8());

        auto *reply = network_manager_->get(req);
        connect(reply, &QNetworkReply::finished, [reply, on_reply] {
            on_reply(reply);
            reply->deleteLater();
        });
    }

    template<typename T>
    auto post(QNetworkRequest req, const T& data, const auto on_reply, const bool authorized = false) -> void {
        const auto token = Auth::instance().get_token();
        if (authorized && token.isEmpty()) {
            Auth::instance().login();
            return; // TODO: err?
        };
        req.setRawHeader("Authorization", "Bearer " + token.toUtf8());

        auto *reply = network_manager_->post(req, data);
        connect(reply, &QNetworkReply::finished, [reply, on_reply] {
            on_reply(reply);
            reply->deleteLater();
        });
    }

private:
    Network() = default;

    std::unique_ptr<QNetworkAccessManager> network_manager_{std::make_unique<QNetworkAccessManager>(nullptr)};
};
