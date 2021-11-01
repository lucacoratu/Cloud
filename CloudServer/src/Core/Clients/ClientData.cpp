#include "cloudpch.h"
#include "ClientData.h"

ClientData::ClientData(const std::string& username)
	: accountUsername(username)
{
	//Initalize member variables
	supportsEncyption = false;
}

ClientData::~ClientData()
{
	this->accountUsername.clear();
}
