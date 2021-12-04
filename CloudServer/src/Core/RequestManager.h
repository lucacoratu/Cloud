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
	static std::map<uint64_t, ClientData*> connectedClients;
public:
	//Connection functions
	static const std::string NewClientConnected(uint64_t clientSocket);
	static void ClientDisconnected(uint64_t clientSocket);
	static const std::string ReceivePublicKey(uint64_t clientSocket, const std::string message);

	//invalid message functions
	static const std::string UnknownRequest(uint64_t clientSocket);
	static const std::string InvalidMessageLength(uint64_t clientSocket, const std::string message);

	//Account functions
	static const std::string RegisterNewAccount(uint64_t clientSocket, const std::vector<std::string>& messageTokens);
	static const std::string LoginIntoAccount(uint64_t clientSocket, const std::vector<std::string>& messageTokens);
	static const std::string LogoutFromAccount(uint64_t clientSocket, const std::vector<std::string>& messageTokens);

	//Working with files functions
	static const std::string ChangeDirectory(uint64_t clientSocket, const std::string& messageData);
	static const std::string CreateNewDirectory(uint64_t clientSocket, const std::string& messageData);
	static const std::string CreateNewFile(uint64_t clientSocket, const std::string& messageData);
	static const std::string ViewDirectoryContent(uint64_t clientSocket, const std::string& messageData);

	//Download functions
	static const std::string DownloadFile(uint64_t clientSocket, const std::string& messageData);
	static const std::string StartTransmission(uint64_t clientSocket, const std::string& messageData);
	static const std::string Acknowledgement(uint64_t clientSocket, const std::string& messageData);

	//Upload functions
	static const std::string UploadFile(uint64_t clientSocket, const std::string& messageData);
	static const std::string ReceiveChunk(uint64_t clientSocket, const std::string& messageData);
	static const std::string LastChunk(uint64_t clientSocket, const std::string& messageData);

	//Accessors
	static bool ClientSupportsEncryption(uint64_t clientSocket);
	static std::string GetClientSecret(uint64_t clientSocket);

	//Deallocate the memory for the remaining users if the server closes
	static void ClearRequestManager();
};

#endif

