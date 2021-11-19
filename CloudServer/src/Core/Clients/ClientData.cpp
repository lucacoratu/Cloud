#include "cloudpch.h"
#include "ClientData.h"

ClientData::ClientData(const std::string publicKey, const std::string privateKey)
	: publicKey(publicKey), privateKey(privateKey)
{
	this->secret = "";
	this->accountUsername = "";
	this->supportsEncyption = false;
}


ClientData::~ClientData()
{
	this->accountUsername.clear();
}

const std::string ClientData::GetPrivateKey() const
{
	return this->privateKey;
}

const std::string ClientData::GetSecret() const
{
	return this->secret;
}

const std::string ClientData::GetAccountUsername() const
{
	return this->accountUsername;
}

const std::string ClientData::GetCurrentDirectory() const
{
	return this->currentDirectory;
}

bool ClientData::SupportsEncryption()
{
	/*
	* If the secret has been generated it means that the client supports encrypted connection
	* The secret is generated when the server receives the public key from the client
	*/
	return (this->secret != "") ? true : false;
}

void ClientData::SetSupportsEncryption(const bool supportsEncryption)
{
	this->supportsEncyption = supportsEncryption;
}

void ClientData::SetSecret(const std::string& secret)
{
	this->secret = secret;
}

void ClientData::SetAccountUsername(const std::string& username)
{
	this->accountUsername = username;
}

void ClientData::ChangeCurrentDirectory(const std::string& currentDirectory)
{
	this->currentDirectory = currentDirectory;
	//std::cout << "Current directory: " << currentDirectory;
}
