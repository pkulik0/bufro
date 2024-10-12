#pragma once

#include <string>

#ifdef DEBUG
const std::string BASE_API_URL = "http://localhost:8080/";
const std::string BASE_AUTH_URL = "http://localhost:8081/";
const std::string AUTH_REALM = "bufro";
#else
const std::string BASE_API_URL = "https://bufro.com/";
const std::string BASE_AUTH_URL = "https://sso.ony.sh/";
const std::string AUTH_REALM = "onysh";
#endif

const std::string AUTH_LOGIN_URL = BASE_AUTH_URL + "realms/" + AUTH_REALM + "/protocol/openid-connect/auth";
const std::string AUTH_TOKEN_URL = BASE_AUTH_URL + "realms/" + AUTH_REALM + "/protocol/openid-connect/token";
const std::string AUTH_USER_INFO_URL = BASE_AUTH_URL + "realms/" + AUTH_REALM + "/protocol/openid-connect/userinfo";

const std::string AUTH_CLIENT_ID = "bufro-client";
const std::string AUTH_CLIENT_SECRET = "3JdESuwZZgGV3rt019bDoOcIsLQuAcXL";

const std::string TRAY_ICON_PATH = ":/assets/tray_icon.png";
const std::string PAUSE_ICON_PATH = ":/assets/pause_icon.png";
const std::string UPLOAD_ICON_PATH = ":/assets/upload_icon.png";
const std::string APP_ICON_PATH = ":/assets/app_icon.png";
