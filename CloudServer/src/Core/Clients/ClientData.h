#ifndef CLIENTDATA_H
#define CLIENTDATA_H

class ClientData {
private:
	//Member variables
	bool supportsEncyption;
	std::string accountUsername;

	//Current directory the user is in
	std::string currentDirectory;

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
	const std::string GetActiveDirectory() const;
	bool SupportsEncryption();

	//Modifiers
	void SetSupportsEncryption(const bool supportsEncryption);
	void SetSecret(const std::string& secret);
	void SetAccountUsername(const std::string& username);
	void ChangeCurrentDirectory(const std::string& currentDirectory);
};


#endif
