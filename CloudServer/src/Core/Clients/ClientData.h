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

	//Accessors
	const std::string GetPrivateKey() const;
	const std::string GetSecret() const;
	const std::string GetAccountUsername() const;
	bool SupportsEncryption();

	//Modifiers
	void SetSupportsEncryption(const bool supportsEncryption);
	void SetSecret(const std::string& secret);
	void SetAccountUsername(const std::string& username);
};


#endif
