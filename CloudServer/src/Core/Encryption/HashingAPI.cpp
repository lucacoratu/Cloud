#include "cloudpch.h"
#include "HashingAPI.h"

//Construct the sha256 static object
SHA256 HashingAPI::shaObject = SHA256();

std::string HashingAPI::HashString(const std::string& str)
{
	/*
	* Hashes the string given in the parameter and returns the hexadecimal representation of the hashed string
	*/
	shaObject.update(str);
	uint8_t* digest = shaObject.digest();
	std::string return_string = shaObject.toString(digest);
	delete[] digest;

	return return_string;
}
