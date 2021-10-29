#include "cloudpch.h"

#include "DatabaseAPI.h"
#include "Core/Log.h"
#include "Errors/ErrorCodes.h"
#include "Core/Database/Queries/Queries.h"


sqlite3* DatabaseAPI::database = nullptr;
std::string DatabaseAPI::databaseRelativePath = "";

int DatabaseAPI::CreateDatabase(const std::string& scriptFilename, const std::string& databaseFilename)
{
	/* Checks if the file exists
	* if it exists, get all of it's content and executes it as an sqlite script to create an empty database with the tables needed
	* If it doesn't exist then logs the error and exists the program (for now)
	*/

	std::ifstream in; in.open(scriptFilename);
	
	if (!in.is_open()) {
		SV_ERROR("Could not find the script file, {0}! Server will close now!", scriptFilename);
		exit(-1);
	}
	
	//The script file has been found
	std::string script = "";
	while (!in.eof()) {
		std::string line;
		std::getline(in, line);
		script += line + " ";
	}

	//Create the database file
	int create_exit = sqlite3_open(databaseFilename.c_str(), &database);

	if (create_exit != SQLITE_OK) {
		//The database file could not be created, and the server should close if this happens
		SV_ERROR("Could not create the database file {0}", databaseFilename);
		exit(-1);
	}

	//Execute the script
	char* message_error = nullptr;
	int script_exit = sqlite3_exec(database, script.c_str(), nullptr, 0, &message_error);

	//If the script does not execute correctly then exit the program with an error
	if (script_exit != SQLITE_OK) {
		SV_ERROR("Could not execute the script from file: {0}", scriptFilename);
		exit(-1);
	}

	SV_INFO("Database has been created! Filename: {0}", databaseFilename);

	return 0;
}

int DatabaseAPI::InitDatabase(const std::string& databaseFilename)
{
	/*Inititalizes the database if the file specified in the parameter exists else tries to find the script that creates the empty database
	* If the script file does not exist an error will be displayed and the execution will end (the server cannot work without the database)
	* TO DO...For later: if the database script does not exist then the server should work with files!!!
	*/
	
	//Open the database if it exists 
	int open_error = sqlite3_open_v2(databaseFilename.c_str(), &database,SQLITE_OPEN_READWRITE, nullptr);
	
	//The database could not be found
	if (open_error != SQLITE_OK) {
		SV_WARN("The database file could not be found! Filename: {0}, trying to create an empty database from a script file {1}", databaseFilename, "./Database/Scripts/create_database_script.txt");
		int create_exit = CreateDatabase("./Database/Scripts/create_database_script.txt", databaseFilename);
	
		if (create_exit == 0) {
			databaseRelativePath = databaseFilename;
		}
	}

	databaseRelativePath = databaseFilename;
	SV_INFO("Connection to the database succesfully established! Database path: {0}", databaseRelativePath);

	return 0;
}

void DatabaseAPI::CloseDatabaseConnection()
{
	int close_exit = sqlite3_close(database);
	if (close_exit != SQLITE_OK) {
		SV_ERROR("Could not close the database connection, database relative path: {0}", databaseRelativePath);
		exit(-1);
	}
	SV_INFO("Closed the connection to the database! Database relative path: {0}", databaseRelativePath);
}

int DatabaseAPI::AddAccountToDatabase(const std::string& username, const std::string& hashedPassword)
{
	/*
	* First checks if the username already exists in the table
	* If the username already exists then the error code specific to that should be returned
	* Else adds a new entry in the Accounts table for the new account
	*/

	sqlite3_stmt* result = nullptr;
	
	//Prepare the sqlite statement to be executed 
	int select_result = sqlite3_prepare_v2(database, SelectAccountQuery(username).GetQueryCharPtr(), -1, &result, 0);
	//Check if the preparation was successful
	if (select_result != SQLITE_OK) {
		SV_WARN("Could not execute the select statement to check if an username is already used! Username: {0}, Database path: {1}", username, databaseRelativePath);
		return static_cast<int>(ServerErrorCodes::COULD_NOT_CHECK_USERNAME_DUPLICATION);
	}

	//If it was successful
	select_result = sqlite3_step(result);
	//Check if the username already exists in the database
	if (select_result == SQLITE_ROW) {
		//The username already exists so the error message should be returned
		return static_cast<int>(ErrorCodes::USERNAME_ALREADY_USED);
	}

	//Username does not exist in the database
	char* error_message = nullptr;
	int insert_exit = sqlite3_exec(database, RegisterAccountQuery(username, hashedPassword).GetQueryCharPtr(), 0, 0, &error_message);
	if (insert_exit != SQLITE_OK) {
		//The insert statement failed
		SV_WARN("Could not insert into the account into the database! Username: {0}, HashedPassword: {1}", username, hashedPassword);
		return static_cast<int>(ServerErrorCodes::COULD_NOT_INSERT_ACCOUNT_INTO_DATABASE);
	}

	return static_cast<int>(ServerErrorCodes::NO_ERROR_FOUND);
}
