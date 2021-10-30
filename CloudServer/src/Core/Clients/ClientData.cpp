#include "cloudpch.h"
#include "ClientData.h"

ClientData::ClientData(const std::string& username)
	: accountUsername(username)
{

}

ClientData::~ClientData()
{
	this->accountUsername.clear();
}
