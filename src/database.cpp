#ifndef DATABASE_CPP
#define DATABASE_CPP

#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#include <vector>
#include "enviroment.cpp"

using namespace utility;


using namespace std;
class database
{
private:
	enviroment* env;
	logger* log;
	sql::Driver* driver;
	sql::Connection* con;
	sql::Statement* stmt;
	sql::ResultSet* res;
	bool status;
public:
	database(enviroment* env,logger* log) {
		this->env = env;

		try {
			driver = get_driver_instance();
			con = driver->connect(env->getString("db_host"), env->getString("db_user"), env->getString("db_pass"));
			con->setSchema(env->getString("db_schema"));
			stmt = con->createStatement();
			log->logRecord(wxString("Connect DB Success"));
			status = true;
		}

		catch (const sql::SQLException& e) {
			log->logRecord(wxString(e.what()));
			status = false;
		}
		
	}
	
	~database() {
		delete res;
		delete stmt;
		delete con;
	}

	database* excutequery(string querysql) {
		res = stmt->executeQuery(querysql);
		return this;
	}

	vector<map<string_t, string_t>> get() {
		vector<map<string_t, string_t>> rows;
		while (res->next()) {
			map<string_t, string_t> row;
			for (int i = 1; i <= res->getMetaData()->getColumnCount(); i++) {
				row[utility::conversions::to_string_t(res->getMetaData()->getColumnName(i))] = utility::conversions::to_string_t(res->getString(i));
			}
			rows.push_back(row);
		}

		
		return rows;
	}

	bool excute(string querysql) {
		try {
			return stmt->execute(sql::SQLString(querysql));
		}
		catch (sql::SQLException e) {
			cout << e.what() << endl;
		}
	}

	bool get_status() {
		return status;
	}
};

#endif
