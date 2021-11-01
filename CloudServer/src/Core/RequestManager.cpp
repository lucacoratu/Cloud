#include "cloudpch.h"
#include "RequestManager.h"

#include "Core/Encryption/HashingAPI.h"
#include "Core/Core.h"
#include "Errors/ErrorCodes.h"


std::map<uint32_t, ClientData*> RequestManager::connectedClients;

const std::string RequestManager::NewClientConnected(uint32_t clientSocket)
{
	/*
	* Operations that need to be done when a new client connects to the server
	* Prepare the slot for the socket in the map of connected clients (initiatlize the pointer with nullptr)
	* Create a welcome message that will be sent back to the client
	*/
	connectedClients[clientSocket] = nullptr;
	MessageCreator::CreateWelcomeMessage();
	SV_INFO("Client connected to server, socket {0}, preparation completed!", clientSocket);
	return MessageCreator::GetLastMessageAsString();
}

void RequestManager::ClientDisconnected(uint32_t clientSocket)
{
	/*
	* Operations that need to be done if a client disconnects from the server
	* Delete the corresponding client data object and the entry in the map for the client socket
	*/
	delete connectedClients[clientSocket];
	connectedClients.erase(clientSocket);
	SV_INFO("Client disconnected from server, socket {0}, corresponding data has been deallocated", clientSocket);
}

const std::string RequestManager::RegisterNewAccount(const std::vector<std::string>& messageTokens)
{
	//Hash the password received from the client
	std::string hashed_password = HashingAPI::HashString(messageTokens[1]);
	/*
	* Try to add the credentials into the database
	* If the database query fails try to add it again
	*/
	int result = DatabaseAPI::AddAccountToDatabase(messageTokens[0], hashed_password);
	if (result == CONVERT_ERROR(ServerErrorCodes::COULD_NOT_CHECK_USERNAME_DUPLICATION) 
		|| result == CONVERT_ERROR(ServerErrorCodes::COULD_NOT_INSERT_ACCOUNT_INTO_DATABASE)
	) 
	{
		result = DatabaseAPI::AddAccountToDatabase(messageTokens[0], messageTokens[1]);
	}

	//If the query fails again then make an assertion
	SV_ASSERT(result > CONVERT_ERROR(ServerErrorCodes::NO_ERROR_FOUND), "Could not register the account into the database!");

	//Construct the afferent message for the client
	if (result == CONVERT_ERROR(ServerErrorCodes::NO_ERROR_FOUND)) {
		MessageCreator::CreateRegisterCompletedMessage();
		SV_INFO("New account has been created! Username: {0}", messageTokens[0]);
	}
	else {
		MessageCreator::CreateRegisterFailedMessage(static_cast<ErrorCodes>(result));
		//SV_WARN("Could not create the account! Username: {0}, Error, code: {1}, Message: {2}", messageTokens[0], result,  )
	}

	return MessageCreator::GetLastMessageAsString();
}

void RequestManager::ClearRequestManager()
{
	for (auto& client : connectedClients) {
		delete client.second;
	}
	connectedClients.clear();
}
