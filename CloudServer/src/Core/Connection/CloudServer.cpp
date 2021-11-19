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
void CloudServer::onClientConnected(uint64_t clientSocket)
{
	std::string welcome_message = RequestManager::NewClientConnected(clientSocket);
	this->sendToClient(clientSocket, welcome_message, welcome_message.size());
}

void CloudServer::onClientDisconnected(uint64_t clientSocket)
{
	RequestManager::ClientDisconnected(clientSocket);

}

//---------------------------------------------------------------------------------------------------------------------------------


//Requests from client application
void CloudServer::onMessageReceived(uint64_t clientSocket, std::string& msg, int length)
{
	//Create the message parser object
	MessageParser message_parser;

	//The result of the action, that has to be sent back to the client
	std::string result = "";
	std::string plain = msg;

	//TO DO...Server should support encrypted connection
	if (RequestManager::ClientSupportsEncryption(clientSocket)) {
		plain = message_parser.DecryptMessage(msg, RequestManager::GetClientSecret(clientSocket));
	}

	//Verify if the length of the message is long enough
	if (plain.size() < sizeof(MessageHeader)) {
		plain = RequestManager::InvalidMessageLength(clientSocket, plain);

		//Send the response after finishing the request back to the client
		this->sendToClient(clientSocket, result, result.size());

		return;
	}

	message_parser.CreateMessageFromString(plain);

	//Get the action specified in the message
	switch (message_parser.GetMessageAction())
	{
	case Action::TEST_CONNECTION:
		//Client requests the server to send back the message that he sent
		result = plain;
		break;
	case Action::RECEIVE_PUBLIC_KEY:
		//Client sends its public key
		result = RequestManager::ReceivePublicKey(clientSocket, message_parser.GetMessageData());
		break;
	case Action::REGISTER_ACCOUNT:
		//Client wants to register a new account
		result = RequestManager::RegisterNewAccount(clientSocket, message_parser.GetMessageTokens());
		break;
	case Action::LOGIN_INTO_ACCOUNT:
		//Client wants to log into an account 
		result = RequestManager::LoginIntoAccount(clientSocket, message_parser.GetMessageTokens());
		break;
	case Action::LOGOUT_FROM_ACCOUNT:
		//Client wants to log out from account
		result = RequestManager::LogoutFromAccount(clientSocket, message_parser.GetMessageTokens());
		break;
	case Action::CHANGE_DIRECTORY:
		//Client wants to change the current directory
		result = RequestManager::ChangeDirectory(clientSocket, message_parser.GetMessageData());
		break;
	case Action::CREATE_NEW_DIRECTORY:
		//Client wants to create a new directory in the current directory
		result = RequestManager::CreateNewDirectory(clientSocket, message_parser.GetMessageData());
		break;
	case Action::CREATE_NEW_FILE:
		//Client wants to create a new file in the current directory
		result = RequestManager::CreateNewFile(clientSocket, message_parser.GetMessageData());
		break;
	case Action::VIEW_DIRECTORY_CONTENT:
		//Client wants to view the current directory's contents
		result = RequestManager::ViewDirectoryContent(clientSocket, message_parser.GetMessageData());
		break;
	default:
		//The action specified by the client is not a valid one
		result = RequestManager::UnknownRequest(clientSocket);
		break;
	}

	//Send the response after finishing the request back to the client
	this->sendToClient(clientSocket, result, result.size());
}
//-----------------------------------------------------------------------------------------------------------