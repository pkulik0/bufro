#pragma once

#include <memory>

#include <QNetworkReply>

#include "auth.hxx"
#include "pb/bufro.pb.h"

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

#define BUF_TYPE_TO_STRING(type) ((type) == BUF_TYPE_IMAGE ? "image" : (type) == BUF_TYPE_VIDEO ? "video" : "unknown")

namespace api {
    inline auto upload(BufType type, const QByteArray& bytes) -> void {
        CreateBufRequest request;
        request.set_data(bytes.constData(), bytes.size());
        request.set_type(type);

        const auto url = BASE_API_URL + "bufs";
        QNetworkRequest req(QUrl(url.data()));
        req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-protobuf");

        Network::instance().post(req, QByteArray::fromStdString(request.SerializeAsString()), [type](QNetworkReply* reply) {
            if (reply->error() != QNetworkReply::NoError) {
                qDebug() << "Error:" << reply->errorString();
                qDebug() << "Reply content:" << reply->readAll();
                return;
            }

            const QByteArray resp = reply->readAll();

            CreateBufResponse response;
            response.ParseFromArray(resp.constData(), static_cast<int>(resp.size()));

            qDebug() << "Created" << BUF_TYPE_TO_STRING(type) << "buf:" << response.id();
            const auto bufUrl = BASE_API_URL + response.id();
            QDesktopServices::openUrl(QUrl(bufUrl.data()));
        }, true);
    }

}