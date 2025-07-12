#pragma once
#include "./wnetwrap.h"
#include "./json.hpp"

using json = nlohmann::json;

using namespace KeyAuth;

std::string name = PicoCrypt("EliteX").decrypt();
std::string ownerid = PicoCrypt("0c3VajlKCq").decrypt();
std::string secret = PicoCrypt("d9987f2748655bce930816aa3e445b2167f8408ec8c340ddfa350eaccda11fbd").decrypt();
std::string version = PicoCrypt("1.0").decrypt();
std::string url = PicoCrypt("https://keyauth.win/api/1.2/").decrypt();

api KeyAuthApp(name, ownerid, secret, version, url);


bool FirstStart = true;
bool LoggedIn = false;
std::string Text;
std::string KeyAuth_username;
std::string KeyAuth_message;
std::string KeyAuth_sessionid;
std::string KeyAuth_version;
std::string KeyAuth_expiry;
std::string KeyAuth_lastlogin;
std::string KeyAuth_createdate;
std::string KeyAuth_ip; 
auto KeyAuth_init_URL_str = ("https://prod.keyauth.com/api/1.3/?type=init&ver=" + version + "&name=" + name +  "&ownerid=" + ownerid);
auto KeyAuth_init_URL = (KeyAuth_init_URL_str.c_str());

char KeyAuth_USER_Key_char[64] = "";
char KeyAuth_USER_user_char[64] = "";
char KeyAuth_USER_pass_char[64] = "";

#include <atlsecurity.h> 

std::string get_hwid() {
	ATL::CAccessToken accessToken;
	ATL::CSid currentUserSid;
	if (accessToken.GetProcessToken(TOKEN_READ | TOKEN_QUERY) &&
		accessToken.GetUser(&currentUserSid))
		return std::string(CT2A(currentUserSid.Sid()));
	return "none";
}

std::string KeyAuth_USER_HWID = get_hwid();
std::string KeyAuth_USER_Key_str(KeyAuth_USER_Key_char);
std::string KeyAuth_USER_pass_str(KeyAuth_USER_pass_char);
std::string KeyAuth_USER_user_str(KeyAuth_USER_user_char);

