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

CheckCredentialsQuery::CheckCredentialsQuery(const std::string& username, const std::string& hashedPassword)
	: Query("SELECT * FROM Accounts WHERE Username = \'" + username + "\' and Password = \'" + hashedPassword + "\';")
{

}


//INSERTION QUERIES
RegisterAccountQuery::RegisterAccountQuery(const std::string& username, const std::string& hashedPassword, const std::string& email)
	: Query("INSERT INTO Accounts(Username, Password, Email, LastActive) VALUES( \'" + username + "\', \'" + hashedPassword + "\', " + "\'" + email +"\', 0);")
{

}

//UPDATE QUERIES


//DELETE QUERIES


