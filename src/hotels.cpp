#ifndef HOTELS_CPP
#define HOTELS_CPP

#include "models.cpp"
class hotels : public models
{
public:
	hotels(string id) : models(id) {
		tablename = "hotels";
		primarykey = "id";
	}

	hotels(map<string_t,string_t> row) : models(row) {
		tablename = "hotels";
		primarykey = "id";
	}
};


#endif