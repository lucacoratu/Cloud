#ifndef SHAAPI_H
#define SHAAPI_H

#include <SHA256.h>

class HashingAPI {
private:
	static SHA256 shaObject;
public:
	//Hashes the string given in the parameter using the sha256 algorithm and returns the hashed string
	static std::string HashString(const std::string& str);
};

#endif
