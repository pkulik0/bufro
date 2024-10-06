#pragma once

#include <QSettings>

class Settings final : public QSettings {
    Q_OBJECT
public:
    static Settings& instance() {
        static Settings instance;
        return instance;
    }

    auto set(const QString& key, const QVariant& value, const bool doSync = true) -> void {
        setValue(key, value);
        if (doSync) sync();
    }

    [[nodiscard]] auto get(const QString& key, const QVariant& default_value = {}) const -> QVariant {
        return value(key, default_value);
    }
private:
    Settings() : QSettings("bufro", "client") {}
};