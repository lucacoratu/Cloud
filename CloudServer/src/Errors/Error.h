#ifndef ERROR_H
#define ERROR_H

#include "Errors/ErrorCodes.h"

class Error {
private:
	//Member variables
	short code;
	std::string details;
public:
	//Functions
	Error(const short& code, const std::string& details);
	~Error();

	//Accessors
	const std::string& GetErrorDetails() const;
	const short& GetErrorCode() const;
};

#endif
