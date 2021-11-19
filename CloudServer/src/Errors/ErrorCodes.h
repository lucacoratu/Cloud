#ifndef ERRORCODES_H
#define ERRORCODES_H

enum class ErrorCodes {
	NO_ERROR_FOUND = 0,
	USERNAME_ALREADY_USED = 1,
	UNKNOWN_REQUEST = 2,
	MESSAGE_TOO_SHORT,
	INVALID_NUMBER_TOKENS,
	ALREADY_LOGGED_IN,
	NOT_LOGGED_IN,
	INEXISTENT_DIRECTORY,
};


enum class ServerErrorCodes {
	NO_ERROR_FOUND = 256,
	COULD_NOT_CHECK_USERNAME_DUPLICATION = 257,
	COULD_NOT_INSERT_ACCOUNT_INTO_DATABASE = 258,
	INEXISTENT_ACCOUNT,
};

#define CONVERT_ERROR(x) static_cast<int>((x))

#endif
