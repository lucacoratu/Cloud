#ifndef SHAAPI_H
#define SHAAPI_H

#include <SHA256.h>

class HashingAPI {
private:
	static SHA256 shaObject;
public:
	static std::string HashString(const std::string& str);
};

#endif
