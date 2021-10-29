#include "cloudpch.h"
#include "Queries.h"


//BASE CLASS FOR QUERIES
Query::Query(const std::string& query)
	: query(query)
{

}

Query::~Query()
{
	this->query.clear();
}

const char* Query::GetQueryCharPtr() const
{
	//Returns the query into the format that sqlite3 supports
	return this->query.c_str();
}


//SELECTION QUERIES
SelectAccountQuery::SelectAccountQuery(const std::string& username)
	: Query("SELECT * FROM Accounts WHERE Username = \'" + username + "\';")
{

}



//INSERTION QUERIES
RegisterAccountQuery::RegisterAccountQuery(const std::string& username, const std::string& hashedPassword)
	: Query("INSERT INTO TABLE Accounts VALUES( \'" + username + "\', \'" + hashedPassword + "\', " + "0);")
{

}

//UPDATE QUERIES


//DELETE QUERIES
