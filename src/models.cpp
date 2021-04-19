#ifndef MODELS_CPP
#define MODELS_CPP

#include "database.cpp"
#include "querybuilder.cpp"
#include <cpprest/json.h>

database* db;
class models
{
protected:
	string tablename;
	string primarykey = "id";
	string id;
	map<string_t, string_t> row;
public:
	models(string id) {
		this->id = id;
	}

	models(map<string_t, string_t> row) {
		this->row = row;
	}

	models(const models &model) {
		tablename = model.tablename;
		primarykey = model.primarykey;
	}

	models* find() {
		vector<map<string_t, string_t>> rows = db->excutequery(
			querybuilder(tablename).selectcolumn("*")->wherecolumn(primarykey, "=", id)->get()
		)
			->get();

		if (rows.size() > 0) row = rows.at(0);
		return this;
	}

	bool exist() {
		return row.size() > 0;
	}

	vector<models*> all() {
		vector<models*> vcts;

		for (map<string_t, string_t>& r : db->excutequery(
			querybuilder(tablename).selectcolumn("*")->get())
			->get()) {
			models* model = new models(*this);
			model->set(r);
			vcts.push_back(model->set(r));
		}
		return vcts;
	}

	bool deletemodel() {
		return db->excute(
			querybuilder(tablename).deleterow()->wherecolumn(primarykey, "=", id)->get()
		);
	}

	models* updatemodel() {
		querybuilder* builder = new querybuilder(tablename);
		for (auto r : row) {
			builder->setupdate(utility::conversions::to_utf8string(r.first), utility::conversions::to_utf8string(r.second));
		}
		db->excute(builder->updaterow()->wherecolumn(primarykey,"=",id)->get());

		
		return this;
	}

	models* insertmodel() {
		querybuilder* builder = new querybuilder(tablename);
		for (auto r : row) {
			builder->insertcolumn(utility::conversions::to_utf8string(r.first), utility::conversions::to_utf8string(r.second));
		}
		db->excute(builder->get());
		find();
		return this;
	}

	models* set(map<string_t, string_t> row) {
		this->row = row;
		id = utility::conversions::to_utf8string(row[utility::conversions::to_string_t(primarykey)]);
		return this;
	}

	models* setid() {
		this->id = utility::conversions::to_utf8string(row[utility::conversions::to_string_t(primarykey)]);
		return this;
	}

	map<string_t, string_t> get() {
		return row;
	}

	string_t get(string_t column) {
		return row[column];
	}

	string getid() {
		return id;
	}

	models* set(string_t column, string_t value) {
		row[column] = value;
		return this;
	}

	web::json::value getjson() {
		web::json::value data;
		for (auto &r : row) {
			data[r.first] = web::json::value::string(r.second);
		}
		return data;
	}
};

#endif