#ifndef REQUESTMANAGER_H
#define REQUESTMANAGER_H

#include "Core/Database/DatabaseAPI.h"
#include "Core/Clients/ClientData.h"
#include "Message/MessageCreator.h"

class RequestManager
{
private:
	//Static member variables
	//A list of connected clients
	static std::map<uint32_t, ClientData*> connectedClients;
public:
	static const std::string NewClientConnected(uint32_t clientSocket);
	static void ClientDisconnected(uint32_t clientSocket);
	//static const std::string UnknownRequest(uint32_t clientSocket, const Message& message);
	static const std::string RegisterNewAccount(const std::vector<std::string>& messageTokens);


	//Deallocate the memory for the remaining users if the server closes
	static void ClearRequestManager();
};

#endif

