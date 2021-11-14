#include "cloudpch.h"
#include "EncryptionAPI.h"

AES EncryptionAPI::aes = AES(128);

static void RemovePadding(std::string& decrypted_string) {
	for (size_t i = decrypted_string.size() - 1; i >= 0; i--) {
		if (decrypted_string[i] == '\0')
			decrypted_string.erase(decrypted_string.begin() + i);
		if (decrypted_string[i] != '\0')
			return;
	}
}

std::string EncryptionAPI::Encrypt(string data, string key)
{
	unsigned char iv[] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
	unsigned int out_len;
	unsigned char* encrypted_data = aes.EncryptCBC((unsigned char*)data.c_str(), static_cast<unsigned int>(data.size()), (unsigned char*)key.c_str(), iv, out_len);

	std::string encrypted = "";
	encrypted.append((const char*)encrypted_data, static_cast<size_t>(out_len));
	return encrypted;
}

std::string EncryptionAPI::Decrypt(const string& data, string key)
{
	//Create a copy of the data until first 0 byte is found
	std::string copy_data = "";
	for (size_t i = 0; i < data.size(); i++) {
		if (data[i] == '\0')
			break;
		copy_data += data[i];
	}

	unsigned char iv[] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
	unsigned char* decrypted_data = aes.DecryptCBC((unsigned char*)copy_data.c_str(), static_cast<unsigned int>(copy_data.size()), (unsigned char*)key.c_str(), iv);

	std::string decrypted = "";
	decrypted.append((const char*)decrypted_data, copy_data.size());
	RemovePadding(decrypted);
	return decrypted;
}
