#ifndef QUERIES_H
#define QUERIES_H

//A set of wrappers over strings in order to be easier to query the database in the DatabaseAPI

//BASE CLASS FOR THE QUERIES
class Query {
private:
	std::string query;
public:
	Query(const std::string& query);
	~Query();
	const char* GetQueryCharPtr() const;
};


//SELECTION QUERIES
class SelectAccountQuery : public Query {
public:
	SelectAccountQuery(const std::string& username);
};


//INSERT QUERIES
class RegisterAccountQuery : public Query {
public:
	RegisterAccountQuery(const std::string& username, const std::string& hashedPassword);
};


//UPDATE QUERIES


//DELETE QUERIES


#endif
