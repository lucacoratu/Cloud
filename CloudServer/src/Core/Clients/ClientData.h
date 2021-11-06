#ifndef CLIENTDATA_H
#define CLIENTDATA_H

class ClientData {
private:
	//TO DO...Remember the client secret (for when encryption will be supported)
	//Member variables
	bool supportsEncyption;
	std::string accountUsername;

	//Diffie-Hellman variables
	std::string secret;
	std::string publicKey;
	std::string privateKey;
public:
	ClientData(const std::string publicKey, const std::string privateKey);
	~ClientData();

	bool SupportsEncryption();
};


#endif
