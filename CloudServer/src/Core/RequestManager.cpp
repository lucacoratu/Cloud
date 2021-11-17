#include "cloudpch.h"
#include "RequestManager.h"

#include "Core/Encryption/HashingAPI.h"
#include "Core/Core.h"
#include "Errors/ErrorCodes.h"
#include "Core/Encryption/DiffieHellmanAPI.h"

std::map<uint64_t, ClientData*> RequestManager::connectedClients;

const std::string RequestManager::NewClientConnected(uint64_t clientSocket)
{
	/*
	* Operations that need to be done when a new client connects to the server
	* Prepare the slot for the socket in the map of connected clients (initiatlize the pointer with nullptr)
	* Create a welcome message, containing the public key of the server, that will be sent back to the client
	*/

	MessageCreator message_creator;

	connectedClients[clientSocket] = nullptr;
	SV_INFO("Client connected to server, socket {0}, preparation completed!", clientSocket);

	//Generate the key pair for the client
	DiffieHellmanKeyPair ServerKeyPair = DiffieHellmanAPI::GenerateKeyPair();

	//DEBUG...Remove later
	SV_PRINT_KEY("Public key", ServerKeyPair.publicKey);
	SV_PRINT_KEY("Private key", ServerKeyPair.privateKey);

	//Save the keys in a more accessible format (std::string instead of unsigned char[])
	std::string publicKey, privateKey;
	for (int i = 0; i < DH_KEY_LENGTH; i++) {
		publicKey += ServerKeyPair.publicKey[i];
		privateKey += ServerKeyPair.privateKey[i];
	}

	connectedClients[clientSocket] = new ClientData(publicKey, privateKey);

	//Create the message for the client, containing the public key
	message_creator.CreatePublicKeyMessage(publicKey);

	return message_creator.GetLastMessageAsString();
}

void RequestManager::ClientDisconnected(uint64_t clientSocket)
{
	/*
	* Operations that need to be done if a client disconnects from the server
	* Delete the corresponding client data object and the entry in the map for the client socket
	*/
	delete connectedClients[clientSocket];
	connectedClients.erase(clientSocket);
	SV_INFO("Client disconnected from server, socket {0}, corresponding data has been deallocated", clientSocket);
}

const std::string RequestManager::UnknownRequest(uint64_t clientSocket)
{
	/*
	* Creates a message for the client which says that the request it asked for is bot supported
	* If the client supports encrypted connection then the message will be encrypted
	* Else it will remain in plain text
	*/
	MessageCreator message_creator;
	
	SV_WARN("Unsupported request from client, socket {0}", clientSocket);
	message_creator.CreateMessage(Action::NO_ACTION, static_cast<char>(ErrorCodes::UNKNOWN_REQUEST), "This type of request is not supported by the server");

	std::string message_for_client = message_creator.GetLastMessageAsString();
	if (connectedClients[clientSocket]->SupportsEncryption())
		message_for_client = message_creator.EncryptMessage(connectedClients[clientSocket]->GetSecret());

	return message_for_client;
}

const std::string RequestManager::InvalidMessageLength(uint64_t clientSocket, const std::string message)
{
	/*
	* Creates an error message for the client
	* If the client supports encryption then the error message will be encrypted
	* Else the message will remain in plain text
	*/
	MessageCreator message_creator;

	SV_WARN("Message received from client was too short, socket {0}", clientSocket);
	std::string invalid_length_message = "The message you sent is too short";
	message_creator.CreateMessage(Action::NO_ACTION, static_cast<char>(ErrorCodes::MESSAGE_TOO_SHORT), invalid_length_message);
	std::string message_for_client = message_creator.GetLastMessageAsString();

	if(connectedClients[clientSocket]->SupportsEncryption())
		message_for_client = message_creator.EncryptMessage(RequestManager::GetClientSecret(clientSocket));

	
	return message_for_client;
}

const std::string RequestManager::RegisterNewAccount(uint64_t clientSocket, const std::vector<std::string>& messageTokens)
{
	MessageCreator message_creator;

	//Check if the client passed enough arguments to the request
	if (messageTokens.size() != 3) {
		message_creator.CreateInvalidNumberOfTokensMessage();
		std::string message_client = message_creator.GetLastMessageAsString();
		if (connectedClients[clientSocket]->SupportsEncryption() == true)
			message_client = message_creator.EncryptMessage(connectedClients[clientSocket]->GetSecret());
		return message_client;
	}

	//Hash the password received from the client
	std::string hashed_password = HashingAPI::HashString(messageTokens[1]);
	/*
	* Try to add the credentials into the database
	* If the database query fails try to add it again
	*/
	int result = DatabaseAPI::AddAccountToDatabase(messageTokens[0], hashed_password, messageTokens[2]);
	if (result == CONVERT_ERROR(ServerErrorCodes::COULD_NOT_CHECK_USERNAME_DUPLICATION) 
		|| result == CONVERT_ERROR(ServerErrorCodes::COULD_NOT_INSERT_ACCOUNT_INTO_DATABASE)
	) 
	{
		result = DatabaseAPI::AddAccountToDatabase(messageTokens[0], hashed_password, messageTokens[2]);
	}

	//TO DO...Check for username duplication

	//If the query fails again then make an assertion
	SV_ASSERT(result == CONVERT_ERROR(ServerErrorCodes::NO_ERROR_FOUND), "Could not register the account into the database!");

	//Construct the afferent message for the client
	if (result == CONVERT_ERROR(ServerErrorCodes::NO_ERROR_FOUND)) {
		message_creator.CreateRegisterCompletedMessage();
		SV_INFO("New account has been created! Username: {0}", messageTokens[0]);
	}
	else {
		message_creator.CreateRegisterFailedMessage(static_cast<ErrorCodes>(result));
		//SV_WARN("Could not create the account! Username: {0}, Error, code: {1}, Message: {2}", messageTokens[0], result,  )
	}

	std::string message_for_client = message_creator.GetLastMessageAsString();
	if (connectedClients[clientSocket]->SupportsEncryption())
		message_for_client = message_creator.EncryptMessage(connectedClients[clientSocket]->GetSecret());

	return message_for_client;
}

const std::string RequestManager::LoginIntoAccount(uint64_t clientSocket, const std::vector<std::string>& messageTokens)
{
	/*
	* Hashes the password received from the client then asks the database if the account sent by the client is valid
	* If it is then the login is successful, otherwise it failed
	*/
	MessageCreator message_creator;

	//Check the number of arguments passed to the command
	if (messageTokens.size() != 2) {
		message_creator.CreateInvalidNumberOfTokensMessage();
		std::string message_client = message_creator.GetLastMessageAsString();
		if (connectedClients[clientSocket]->SupportsEncryption() == true)
			message_client = message_creator.EncryptMessage(connectedClients[clientSocket]->GetSecret());
		return message_client;
	}

	SV_INFO("Client, socket: {0}, requested to login into account: {1}", clientSocket, messageTokens[0]);

	std::string hashed_password = HashingAPI::HashString(messageTokens[1]);
	bool result = DatabaseAPI::CheckCredentials(messageTokens[0], hashed_password);

	if (result == false) {
		message_creator.CreateLoginFailedMessage();
		SV_WARN("Client, socket: {0}, failed to login, inexistent account or wrong credentials", clientSocket);
	}
	else {
		message_creator.CreateMessage(Action::NO_ACTION, static_cast<char>(0), "Login successful");
		SV_INFO("Client, socket: {0}, logged into account, username: {1}", clientSocket, messageTokens[0]);

		//Check if the client isn't already logged in
		if (connectedClients[clientSocket]->GetAccountUsername() != "") {
			SV_WARN("Client, socket: {0}, failed to login, account already online, username: {1}", clientSocket, messageTokens[0]);
			message_creator.CreateMessage(Action::NO_ACTION, static_cast<char>(ErrorCodes::ALREADY_LOGGED_IN), "Account is already in use!");

			std::string message_client = message_creator.GetLastMessageAsString();
			if (connectedClients[clientSocket]->SupportsEncryption() == true)
				message_client = message_creator.EncryptMessage(connectedClients[clientSocket]->GetSecret());
			return message_client;
		}

		//Populate the client data structure
		connectedClients[clientSocket]->SetAccountUsername(messageTokens[0]);
	}
	
	std::string message_for_client = message_creator.GetLastMessageAsString();
	if (connectedClients[clientSocket]->SupportsEncryption())
		message_for_client = message_creator.EncryptMessage(connectedClients[clientSocket]->GetSecret());

	return message_for_client;
}

const std::string RequestManager::ReceivePublicKey(uint64_t clientSocket, const std::string message)
{
	/*
	* Receives the public key of the client then generates the shared secret
	* And saves the secret in the client data of the afferent client
	* Then points out that the client supports encrypted connection
	*/
	MessageCreator message_creator;

	DH_KEY clientPublicKey;
	for (size_t i = 0; i < message.size(); i++)
		clientPublicKey[i] = message[i];

	DH_KEY serverPrivateKey;
	std::string serverPrivate = connectedClients[clientSocket]->GetPrivateKey();
	for (size_t i = 0; i < serverPrivate.size(); i++)
		serverPrivateKey[i] = serverPrivate[i];

	//Generate the secret
	DH_KEY secret;
	DiffieHellmanAPI::GenerateSecret(secret, serverPrivateKey, clientPublicKey);
	std::string secretKey = "";

	for (size_t i = 0; i < DH_KEY_LENGTH; i++)
		secretKey += secret[i];

	connectedClients[clientSocket]->SetSupportsEncryption(true);
	connectedClients[clientSocket]->SetSecret(secretKey);

	SV_INFO("Secret generated for client, socket {0}", clientSocket);
	_print_key("Secret", secret);

	message_creator.CreateReceivedPublicKey();

	return message_creator.GetLastMessageAsString();
}

bool RequestManager::ClientSupportsEncryption(uint64_t clientSocket)
{
	/*
	* Returns true if the client supports encrypted connection
	* Else this returns false
	*/
	return connectedClients[clientSocket]->SupportsEncryption();
}

std::string RequestManager::GetClientSecret(uint64_t clientSocket)
{
	/*
	* Returns the established secret with the client
	*/
	return connectedClients[clientSocket]->GetSecret();
}

void RequestManager::ClearRequestManager()
{
	/*
	* Deallocates the memory used by the request manager
	*/
	for (auto& client : connectedClients) {
		delete client.second;
	}
	connectedClients.clear();
}
