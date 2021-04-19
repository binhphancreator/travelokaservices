#ifndef QUERYBUILDER_CPP
#define QUERYBUILDER_CPP

#include <string>
#include <vector>
using namespace std;
class querybuilder
{
private:
	string table;
	string select="SELECT ";
	string where = "WHERE ";
	string insert = "INSERT INTO ";
	string deletefrom = "";
	string update = "";
	string set = "SET ";

	vector<string> insertcols;
	vector<string> insertvals;

	vector<string> updatecols;
	vector<string> updatevals;

	string sql = "";

	void buildinsertsql() {
		insert += table;
		string cols = "";
		string vals = "";
		for (int i = 0; i < insertcols.size(); i++)
		{
			cols += insertcols.at(i);
			vals += "'" + insertvals.at(i) + "'";
			if (i != insertcols.size() - 1) {
				cols += ",";
				vals += ",";
			}
		}
		cols = "(" + cols + ")";
		vals = "(" + vals + ")";
		insert += " " + cols + " VALUES " + vals;
	}

	void buildupdatesql() {
		update += table;
		for (int i = 0; i < updatecols.size(); i++) {
			set += updatecols.at(i) + "=" + "'" + updatevals.at(i) + "'";
			if (i != updatecols.size() - 1) set += ",";
		}
	}
public:
	querybuilder(string table) {
		this->table = table;
	}

	~querybuilder(){}

	querybuilder* selectcolumns(vector<string> columns) {
		for (int i = 0; i < columns.size(); i++)
		{
			this->select += columns.at(i);
			if (i != columns.size() - 1) this->select += ",";
		}
		return this;
	}

	querybuilder* selectcolumn(string column) {
		this->select += this->select == "SELECT " ? "" : ",";
		this->select += column;
		return this;
	}

	querybuilder* wherecolumns(vector<string> columns,vector<string> conditions, vector<string> value) {
		for (int i = 0; i < conditions.size(); i++)
		{
			this->where += columns.at(i) + " " + conditions.at(i) + " " + "'" + value.at(i) + "'";
			if (i != conditions.size() - 1) select += " and ";
		}
		return this;
	}

	querybuilder* wherecolumn(string column, string condition, string value) {
		this->where+= this->where == "WHERE " ? "" : " and ";
		this->where += column + " " + condition + " " + "'" + value + "'";
		return this;
	}

	querybuilder* insertcolumn(string column, string value) {
		insertcols.push_back(column);
		insertvals.push_back(value);
		return this;
	}

	querybuilder* deleterow() {
		deletefrom = "DELETE FROM ";
		return this;
	}

	querybuilder* updaterow() {
		update = "UPDATE ";
		return this;
	}

	querybuilder* setupdate(string column, string value) {
		updatecols.push_back(column);
		updatevals.push_back(value);
		return this;
	}

	string get() {
		sql = this->select + " FROM " 
			+ table + " " 
			+ (this->where=="WHERE " ? "":this->where);

		if (!deletefrom.empty()) {
			sql = deletefrom
				+table + " "
				+(this->where == "WHERE " ? "" : this->where);
		}

		if (!update.empty()) {
			buildupdatesql();
			sql = update + " "
				+set +" "
				+ (this->where == "WHERE " ? "" : this->where);
		}
		
		if (insertcols.size() != 0) {
			buildinsertsql();
			sql = insert;
		}
		return sql;
	}
};

#endif