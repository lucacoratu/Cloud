#include "cloudpch.h"

#include "CloudServer.h"
#include "Log.h"

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
}

void CloudServer::onClientDisconnected(uint32_t clientSocket)
{
	SV_INFO("Client disconnected, socket: {0}", clientSocket);

	//When a client disconnects from the server
	//this->actionHandler.clientDisconnected(clientSocket);
}

//---------------------------------------------------------------------------------------------------------------------------------


//Requests from client application
void CloudServer::onMessageReceived(uint32_t clientSocket, std::string& msg, int length)
{
	////The result of the action, that has to be sent back to the client
	//std::string result;

	////The message to be parsed by the message parser
	//std::string plain = msg;

	////Decrypt the message received from the client if it supports encryption and the diffie-hellman key exchange has finished
	//if (this->actionHandler.SupportsEncryptedConnection(clientSocket)) {
	//	if (this->actionHandler.DiffieHellmanEstablished(clientSocket))
	//		plain = MessageParser::decryptString(msg, this->actionHandler.getClientSecret(clientSocket));
	//}

	////Parse the message
	//switch (this->messageParser.parseMessage(plain))
	//{
	//case SupportedActions::RECEIVE_PUBLIC_KEY:
	//	result = this->actionHandler.receiveClientPublicKey(clientSocket, this->messageParser.getParsedMessage());
	//	break;
	//case SupportedActions::LOGIN_INTO_ACCOUNT:
	//	result = this->actionHandler.loginIntoAccount(clientSocket, this->messageParser.getTokens());
	//	break;
	//case SupportedActions::REGISTER_ACCOUNT:
	//	result = this->actionHandler.registerAccount(clientSocket, this->messageParser.getTokens());
	//	break;
	//case SupportedActions::REQUEST_ACCOUNT_DETAILS:
	//	result = this->actionHandler.requestAccountDetails(clientSocket, this->messageParser.getTokens());
	//	break;
	//case SupportedActions::CREATE_NEW_SURVEY:
	//	result = this->actionHandler.createNewSurvey(clientSocket, this->messageParser.getQuestionsAndAnswears());
	//	break;
	//case SupportedActions::VIEW_ALL_SURVEYS_PREVIEWS:
	//	result = this->actionHandler.viewAllSurveyPreviews(clientSocket);
	//	break;
	//case SupportedActions::REQUEST_SURVEY:
	//	result = this->actionHandler.requestSurvey(clientSocket, this->messageParser.getTokens());
	//	break;
	//case SupportedActions::RECEIVE_COMPLETED_SURVEY:
	//	result = this->actionHandler.receiveCompletedSurvey(clientSocket, this->messageParser.getTokens(ANSWEAR_SEPARATOR));
	//	break;
	//default:
	//	result = this->actionHandler.failedToDecrypt(clientSocket, plain);
	//	break;
	//}

	//this->sendToClient(clientSocket, result, result.size());
	//SV_INFO("Message sent to client, socket {0}", clientSocket);
}
//-----------------------------------------------------------------------------------------------------------