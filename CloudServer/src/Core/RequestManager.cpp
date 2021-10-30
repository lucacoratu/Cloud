#include "cloudpch.h"
#include "RequestManager.h"

#include "Core/Encryption/ShaAPI.h"
#include "Core/Log.h"


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
