#pragma once

#include <string>
#include <string_view>

constexpr std::string BASE_API_URL = "http://localhost:8080/";

constexpr std::string BASE_AUTH_URL = "http://localhost:8081/";
const std::string AUTH_LOGIN_URL = BASE_AUTH_URL + "realms/bufro/protocol/openid-connect/auth";
const std::string AUTH_TOKEN_URL = BASE_AUTH_URL + "realms/bufro/protocol/openid-connect/token";
const std::string AUTH_USER_INFO_URL = BASE_AUTH_URL + "realms/bufro/protocol/openid-connect/userinfo";

constexpr std::string_view AUTH_CLIENT_ID = "app";
constexpr std::string_view AUTH_CLIENT_SECRET = "YITlLPjL8kpXynn6qIPOaEWIXwHlHavo";

constexpr std::string_view ICON_PATH = "/Users/pkulik/repos/bufro/client/assets/icon.png";