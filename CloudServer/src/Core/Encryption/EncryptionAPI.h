#ifndef ENCRYPTIONAPI_H
#define ENCRYPTIONAPI_H

#include <AES.h>

class EncryptionAPI
{
private:
	static AES aes;
public:
	static std::string Encrypt(string data, string key);
	static std::string Decrypt(const string& data, string key);
};

#endif

