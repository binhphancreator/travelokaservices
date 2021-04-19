#ifndef ROUTER_CPP
#define ROUTER_CPP

#include <cpprest/uri.h>
#include <cpprest/http_listener.h>
#include <cpprest/asyncrt_utils.h>
#include <cpprest/json.h>
#include <string>
#include <map>
#include <iostream>
#include "enviroment.cpp"
#include "logger.cpp"


using namespace web;
using namespace http;
using namespace utility;
using namespace web::http;
using namespace http::experimental::listener;

using namespace std;

typedef json::value(*basecontroller)(map<string_t, string_t>);
class router
{
private:
	http_listener listener;
	enviroment* env;
	map < string, basecontroller> controllers;
	map<string, string> methods;
	logger* log;
	bool isstoped = true;
	wxString status;

	void handleget(http_request req) {
		if (conversions::to_utf8string(req.absolute_uri().path()) != "/favicon.ico")
			log->logRecord(req_to_string(req));

		map<string_t, string_t> params = uri::split_query(req.request_uri().query());
		string path = conversions::to_utf8string(req.request_uri().path());
		if (controllers[path] != NULL) {
			if (methods["GET"+path] == "#")
				req.reply(status_codes::OK, controllers[path](params));

			else req.reply(status_codes::MethodNotAllowed);
		}
		else
			req.reply(status_codes::NotFound);
	};

	void handlepost(http_request req) {
		if (conversions::to_utf8string(req.absolute_uri().path()) != "/favicon.ico")
			log->logRecord(req_to_string(req));

		map<string_t, string_t> params = uri::split_query(req.request_uri().query());

		string path = conversions::to_utf8string(req.request_uri().path());
		if (controllers[path] != NULL) {
			if (methods["POST" + path] == "#")
				req.reply(status_codes::OK, controllers[path](params));

			else req.reply(status_codes::MethodNotAllowed);
		}
		else
			req.reply(status_codes::NotFound);
	}

	void handledelete(http_request req) {
		if (conversions::to_utf8string(req.absolute_uri().path()) != "/favicon.ico")
			log->logRecord(req_to_string(req));

		map<string_t, string_t> params = uri::split_query(req.request_uri().query());
		string path = conversions::to_utf8string(req.request_uri().path());
		if (controllers[path] != NULL) {
			if (methods["DELETE" + path] == "#")
				req.reply(status_codes::OK, controllers[path](params));

			else req.reply(status_codes::MethodNotAllowed);
		}
		else
			req.reply(status_codes::NotFound);
	}

	wxString req_to_string(http_request req) {
		wxString reqstr;
		reqstr = wxString::Format("METHOD: %s ; URI: %s", req.method(),req.absolute_uri().to_string());
		return reqstr;
	}
public:
	router(enviroment* env, logger* log)  {
		this->env = env;
		this->log = log;
	}

	void config() {
		listener = http_listener(conversions::to_string_t("http://" + env->getString("host") + ":" + env->getString("port")));
		listener.support(methods::GET, [&](http_request req) { handleget(req); });
		listener.support(methods::POST, [&](http_request req) { handlepost(req); });
		listener.support(methods::DEL, [&](http_request req) { handledelete(req); });
	}

	void listen() {
		try {
			listener.open()
				.then([&]() {
					isstoped = false;
					status =  "Starting to listen on : " + listener.uri().to_string();
					log->logRecord(status);
				})
				.wait();
			while (!isstoped);
		}

		catch (exception const& e) {
			status = wxString(e.what());
			log->logRecord(status);
		}
	}

	void bind(string method, string name, basecontroller c) {
		methods[method+name]="#";
		controllers[name] = c;
	}

	void close() {
		listener.close();
	}

	void stop() {
		if (!isRunning()) {
			status = "Service is not running";
			log->logRecord(status);
			return;
		}

		status = "Close to listen";
		log->logRecord(status);

		this->isstoped = true;
	}

	bool isRunning() {
		return !isstoped;
	}

	~router() {}
};

#endif // !ROUTER_CPP