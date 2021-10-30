#include "cloudpch.h"
#include "CloudServer.h"

#include "Message/MessageParser.h"
#include "Core/RequestManager.h"

CloudServer* CloudServer::instance = nullptr;

//Instance functions
CloudServer& CloudServer::createInstance(const char* ipAdress, int port) {
	if (instance == nullptr) {
		instance = new CloudServer(ipAdress, port);
		return *instance;
	}
	return *instance;
}

void CloudServer::deleteInstance() {
	if (instance != nullptr) {
		delete instance;
		instance = nullptr;
	}
	instance = nullptr;
}


//When a client application connects / disconnects
void CloudServer::onClientConnected(uint32_t clientSocket)
{
	//Server public key
	//std::string publicKey = this->actionHandler.newClientConnected(clientSocket);

	//Send the public key to the client
	//this->sendToClient(clientSocket, publicKey, DH_KEY_LENGTH + 1);
	//SV_INFO("Public key was sent to client");
	std::string welcome_message = RequestManager::NewClientConnected(clientSocket);
	this->sendToClient(clientSocket, welcome_message, welcome_message.size());
}

void CloudServer::onClientDisconnected(uint32_t clientSocket)
{
	RequestManager::ClientDisconnected(clientSocket);
	//When a client disconnects from the server
	//this->actionHandler.clientDisconnected(clientSocket);
}

//---------------------------------------------------------------------------------------------------------------------------------


//Requests from client application
void CloudServer::onMessageReceived(uint32_t clientSocket, std::string& msg, int length)
{
	////Decrypt the message received from the client if it supports encryption and the diffie-hellman key exchange has finished
	//if (this->actionHandler.SupportsEncryptedConnection(clientSocket)) {
	//	if (this->actionHandler.DiffieHellmanEstablished(clientSocket))
	//		plain = MessageParser::decryptString(msg, this->actionHandler.getClientSecret(clientSocket));
	//}

	//The result of the action, that has to be sent back to the client
	std::string result = "";

	//TO DO...Server should support encrypted connection
	std::string plain = msg;
	MessageParser::CreateMessageFromString(plain);

	//Get the action specified in the message
	switch (MessageParser::GetMessageAction()) 
	{
	case Action::TEST_CONNECTION:
		//Client requests the server to send back the message that he sent
		result = plain;
		break;
	default:
		SV_WARN("Unsupported request from client, socket {0}", clientSocket);
		result = "";
		break;
	}

	//Send the response after finishing the request back to the client
	this->sendToClient(clientSocket, result, result.size());
	SV_INFO("Request, type: {0}, completed! Sending the response to the client, socket{1]", MessageParser::GetMessageAction(), clientSocket);
}
//-----------------------------------------------------------------------------------------------------------