#ifndef DATABASEAPI_H
#define DATABASEAPI_H

#include <sqlite3.h>

/* 
*	This class will be the API over the sqlite database 
*/

class DatabaseAPI {
private:
	//Pointer to the database
	static sqlite3* database;
	static std::string databaseRelativePath;

	//Creates the database based on a script in a file
	static int CreateDatabase(const std::string& scriptFilename, const std::string& databaseFilename);
public:
	//Intitializes the database if it exists else it creates an empty database
	static int InitDatabase(const std::string& databaseFilename);

	//Closes the database connection
	static void CloseDatabaseConnection();

	//Adds a new entry to the database with the new account
	static int AddAccountToDatabase(const std::string& username, const std::string& hashedPassword);

	//Checks the credentials of the user for login
	static bool CheckCredentials(const std::string& username, const std::string& hashedPassword);
};

#endif
