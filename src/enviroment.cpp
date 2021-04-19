#ifndef __ENVIROMENT_CPP__
#define __ENVIROMENT_CPP__


#include <map>
#include <string>

using namespace std;

class enviroment {
private:
	map<string,wxString> configure;
public:
	enviroment(){
		configure["host"]= wxString("localhost");
		configure["port"]= wxString("8080");

		configure["db_host"] = wxString("freedb.tech:3306");
		configure["db_user"] = wxString("freedbtech_traveloka");
		configure["db_pass"] = wxString("binhphan2k");
		configure["db_schema"] = wxString("freedbtech_travelokaservice");
	}
	~enviroment(){}

	wxString get(string key) {
		return configure[key];
	}

	string getString(string key) {
		return configure[key].ToStdString();
	}

	enviroment* set(string key, wxString value) {
		configure[key] = value;
		return this;
	}
};

#endif // !__ENVIROMENT_CPP__