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

	static const std::string UnknownRequest(uint32_t clientSocket);
	static const std::string InvalidMessageLength(uint32_t clientSocket, const std::string message);

	static const std::string RegisterNewAccount(uint32_t clientSocket, const std::vector<std::string>& messageTokens);
	static const std::string LoginIntoAccount(uint32_t clientSocket, const std::vector<std::string>& messageTokens);
	static const std::string ReceivePublicKey(uint32_t clientSocket, const std::string message);

	//Accessors
	static bool ClientSupportsEncryption(uint32_t clientSocket);
	static std::string GetClientSecret(uint32_t clientSocket);

	//Deallocate the memory for the remaining users if the server closes
	static void ClearRequestManager();
};

#endif

