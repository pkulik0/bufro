#pragma once

#include <string>
#include <string_view>

#ifdef DEBUG
constexpr std::string BASE_API_URL = "http://localhost:8080/";
constexpr std::string BASE_AUTH_URL = "http://localhost:8081/";
constexpr std::string AUTH_REALM = "bufro";
#else
constexpr std::string BASE_API_URL = "https://bufro.com/";
constexpr std::string BASE_AUTH_URL = "https://sso.ony.sh/";
constexpr std::string AUTH_REALM = "onysh";
#endif

const std::string AUTH_LOGIN_URL = BASE_AUTH_URL + "realms/" + AUTH_REALM + "/protocol/openid-connect/auth";
const std::string AUTH_TOKEN_URL = BASE_AUTH_URL + "realms/" + AUTH_REALM + "/protocol/openid-connect/token";
const std::string AUTH_USER_INFO_URL = BASE_AUTH_URL + "realms/" + AUTH_REALM + "/protocol/openid-connect/userinfo";

constexpr std::string_view AUTH_CLIENT_ID = "bufro-client";
constexpr std::string_view AUTH_CLIENT_SECRET = "3JdESuwZZgGV3rt019bDoOcIsLQuAcXL";

constexpr std::string_view TRAY_ICON_PATH = ":/assets/tray_icon.png";
constexpr std::string_view APP_ICON_PATH = ":/assets/app_icon.png";
