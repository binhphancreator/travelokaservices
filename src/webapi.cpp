#ifndef WEBAPI_CPP
#define WEBAPI_CPP

#include "router.cpp"
#include "hotels.cpp"
class webapi
{
public:
	static void registerapi(router* route) {
		route->bind("GET", "/", [](map<string_t, string_t> params) {
			json::value jsonResponse;
			jsonResponse[L"status"] = json::value::string(utility::conversions::to_string_t("OK"));
			jsonResponse[L"APIs"] = json::value::array();

			jsonResponse[L"APIs"][0][L"method"] = json::value::string(L"GET");
			jsonResponse[L"APIs"][0][L"path"] = json::value::string(L"/hotels");

			jsonResponse[L"APIs"][1][L"method"] = json::value::string(L"GET");
			jsonResponse[L"APIs"][1][L"path"] = json::value::string(L"/detail/hotel?id=?");

			return jsonResponse;
		});

		route->bind("GET", "/hotels", [](map<string_t, string_t> params) {
			json::value jsonResponse;
			jsonResponse[L"status"] = json::value::string(utility::conversions::to_string_t("OK"));
			vector<json::value> listhotels;
			for (auto r : hotels("").all()) {
				listhotels.push_back(r->getjson());
			}
			jsonResponse[L"data"] = json::value::array(listhotels);
			return jsonResponse;
		});

		route->bind("GET", "/detail/hotel", [](map<string_t, string_t> params) {
			json::value jsonResponse;
			jsonResponse[L"status"] = json::value::string(utility::conversions::to_string_t("OK"));
			jsonResponse[L"data"] = json::value::array();
			if (!params[L"id"].empty())
				jsonResponse[L"data"][0] = hotels(utility::conversions::to_utf8string(params[L"id"])).find()->getjson();
			return jsonResponse;
		});

	}
};

#endif