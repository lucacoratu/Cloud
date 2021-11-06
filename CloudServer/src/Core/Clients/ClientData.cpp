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

bool ClientData::SupportsEncryption()
{
	/*
	* If the secret has been generated it means that the client supports encrypted connection
	* The secret is generated when the server receives the public key from the client
	*/
	return (this->secret != "") ? true : false;
}
