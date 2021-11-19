#include "cloudpch.h"
#include "RequestManager.h"

#include "Core/Encryption/HashingAPI.h"
#include "Core/Core.h"
#include "Errors/ErrorCodes.h"
#include "Core/Encryption/DiffieHellmanAPI.h"
#include "Core/FilesystemAPI.h"

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
	/*
	* Registers a new account in the database if the username can't be found in the database
	* Else it send an error message back to the client
	*/
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
	if (result == CONVERT_ERROR(ErrorCodes::USERNAME_ALREADY_USED)) {
		message_creator.CreateMessage(Action::NO_ACTION, static_cast<char>(result), "An account with this username already exists");
		SV_WARN("Client, socket {0}, tried to register an account with an already existing username!", clientSocket);
	}

	//If the query fails again then make an assertion
	SV_ASSERT(result == CONVERT_ERROR(ServerErrorCodes::NO_ERROR_FOUND), "Could not register the account into the database!");

	//Construct the afferent message for the client
	if (result == CONVERT_ERROR(ServerErrorCodes::NO_ERROR_FOUND)) {
		message_creator.CreateRegisterCompletedMessage();
		SV_INFO("New account has been created! Username: {0}", messageTokens[0]);

		//Create the home directory for the user inside the entry directory
		FilesystemAPI::CreateUserDirectory(messageTokens[0]);
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

		//Set the current directory to the home directory
		connectedClients[clientSocket]->ChangeCurrentDirectory("./entry/" + messageTokens[0] + "/");
	}
	
	std::string message_for_client = message_creator.GetLastMessageAsString();
	if (connectedClients[clientSocket]->SupportsEncryption())
		message_for_client = message_creator.EncryptMessage(connectedClients[clientSocket]->GetSecret());

	return message_for_client;
}

const std::string RequestManager::LogoutFromAccount(uint64_t clientSocket, const std::vector<std::string>& messageTokens)
{
	/*
	* Logs out the user from the account it is currently on
	* If the user is not connected on an account then send an error message back to it
	*/

	MessageCreator message_creator;
	if (messageTokens.size() != 1) {
		message_creator.CreateInvalidNumberOfTokensMessage();
		SV_WARN("Client, socket {0}, invalid number of arguments passed to logout", clientSocket);
		std::string message_client = message_creator.GetLastMessageAsString();
		if (connectedClients[clientSocket]->SupportsEncryption() == true)
			message_client = message_creator.EncryptMessage(connectedClients[clientSocket]->GetSecret());
		return message_client;
	}

	SV_INFO("Client, socket {0}, requested to log out of account {1}", clientSocket, messageTokens[0]);

	//Check if the user is connected on an account
	if (connectedClients[clientSocket]->GetAccountUsername() == "") {
		//It is not connected on an account
		message_creator.CreateMessage(Action::NO_ACTION, static_cast<char>(ErrorCodes::NOT_LOGGED_IN), "Can't log out if you are not logged into an account!");
		SV_WARN("Client, socket {0}, couldn't log out of account because it is not logged in", clientSocket);
		std::string message_client = message_creator.GetLastMessageAsString();
		if (connectedClients[clientSocket]->SupportsEncryption() == true)
			message_client = message_creator.EncryptMessage(connectedClients[clientSocket]->GetSecret());
		return message_client;
	}

	//User can log out
	connectedClients[clientSocket]->SetAccountUsername("");
	message_creator.CreateMessage(Action::NO_ACTION, static_cast<char>(ErrorCodes::NO_ERROR_FOUND), "Succesfully logged out of account");
	std::string message_client = message_creator.GetLastMessageAsString();
	if (connectedClients[clientSocket]->SupportsEncryption() == true)
		message_client = message_creator.EncryptMessage(connectedClients[clientSocket]->GetSecret());
	return message_client;
}

const std::string RequestManager::ChangeDirectory(uint64_t clientSocket, const std::string& messageData)
{
	/*
	* Changes the current directory of the client, if the directory exists
	* Else it returns an error message
	*/
	MessageCreator message_creator;

	//Check if the data from the client is correct
	if (messageData == "") {
		message_creator.CreateInvalidNumberOfTokensMessage();
		SV_WARN("Client, socket {0}, invalid number of arguments passed to change directory", clientSocket);
		std::string message_client = message_creator.GetLastMessageAsString();
		if (connectedClients[clientSocket]->SupportsEncryption() == true)
			message_client = message_creator.EncryptMessage(connectedClients[clientSocket]->GetSecret());
		return message_client;
	}

	std::string newPath = connectedClients[clientSocket]->GetActiveDirectory() + " " + messageData;
	SV_INFO("Client, socket: {0}, requested to change the current directory {1} to {2}", clientSocket, connectedClients[clientSocket]->GetActiveDirectory(), newPath);

	//Check if the directory exists
	if (FilesystemAPI::ExistsDirectory(newPath) == false) {
		SV_WARN("Client, socket: {0}, couldn't change the current directory, inexistent next directory {1}!", clientSocket, messageData);
		message_creator.CreateMessage(Action::NO_ACTION, static_cast<char>(ErrorCodes::INEXISTENT_DIRECTORY), "Couldn't change the current directory, next directory doesn't exist!");

		std::string message_client = message_creator.GetLastMessageAsString();
		if (connectedClients[clientSocket]->SupportsEncryption() == true)
			message_client = message_creator.EncryptMessage(connectedClients[clientSocket]->GetSecret());
		return message_client;
	}

	//Change the current directory
	SV_INFO("Client, socket: {0}, changed the current directory from {1} to {2}", clientSocket, connectedClients[clientSocket]->GetActiveDirectory(), newPath);
	connectedClients[clientSocket]->ChangeCurrentDirectory(newPath + "/");

	message_creator.CreateMessage(Action::NO_ACTION, static_cast<char>(0), "Changed the current directory!");
	std::string message_client = message_creator.GetLastMessageAsString();
	if (connectedClients[clientSocket]->SupportsEncryption() == true)
		message_client = message_creator.EncryptMessage(connectedClients[clientSocket]->GetSecret());
	return message_client;
}

const std::string RequestManager::CreateNewDirectory(uint64_t clientSocket, const std::string& messageData)
{
	/*
	* Creates a new directory if one with the same name doesn't exist
	* If the name is already used return a error message
	*/
	MessageCreator message_creator;

	if (messageData == "") {
		message_creator.CreateInvalidNumberOfTokensMessage();
		SV_WARN("Client, socket {0}, invalid number of arguments passed to create new directory", clientSocket);
		std::string message_client = message_creator.GetLastMessageAsString();
		if (connectedClients[clientSocket]->SupportsEncryption() == true)
			message_client = message_creator.EncryptMessage(connectedClients[clientSocket]->GetSecret());
		return message_client;
	}

	std::string newPath = connectedClients[clientSocket]->GetActiveDirectory() + " " + messageData;
	SV_INFO("Client, socket: {0}, requested to create a directory {1} in {2}", clientSocket, messageData, connectedClients[clientSocket]->GetActiveDirectory());

	//Check if the directory exists
	if (FilesystemAPI::ExistsDirectory(newPath) == true) {
		SV_WARN("Client, socket: {0}, couldn't create the directory {1}, one with the same name already exists!", clientSocket, messageData);
		message_creator.CreateMessage(Action::NO_ACTION, static_cast<char>(ErrorCodes::INEXISTENT_DIRECTORY), "Couldn't create the directory, it already exists!");

		std::string message_client = message_creator.GetLastMessageAsString();
		if (connectedClients[clientSocket]->SupportsEncryption() == true)
			message_client = message_creator.EncryptMessage(connectedClients[clientSocket]->GetSecret());
		return message_client;
	}

	FilesystemAPI::CreateNewDirectory(newPath);
	SV_INFO("Client, socket: {0}, created directory {1} in {2}", clientSocket, messageData, connectedClients[clientSocket]->GetActiveDirectory());
	
	message_creator.CreateMessage(Action::NO_ACTION, static_cast<char>(0), "Created the directory!");
	std::string message_client = message_creator.GetLastMessageAsString();
	if (connectedClients[clientSocket]->SupportsEncryption() == true)
		message_client = message_creator.EncryptMessage(connectedClients[clientSocket]->GetSecret());
	return message_client;
}

const std::string RequestManager::CreateNewFile(uint64_t clientSocket, const std::string& messageData)
{
	/*
	* Creates a file in the current directory of the user if another one with the same name doesn't exist
	* Else it returns an error message
	*/
	MessageCreator message_creator;

	if (messageData == "") {
		message_creator.CreateInvalidNumberOfTokensMessage();
		SV_WARN("Client, socket {0}, invalid number of arguments passed to create new directory", clientSocket);
		std::string message_client = message_creator.GetLastMessageAsString();
		if (connectedClients[clientSocket]->SupportsEncryption() == true)
			message_client = message_creator.EncryptMessage(connectedClients[clientSocket]->GetSecret());
		return message_client;
	}

	std::string newPath = connectedClients[clientSocket]->GetActiveDirectory() + " " + messageData;
	SV_INFO("Client, socket: {0}, requested to create a file: {1} in {2}", clientSocket, messageData, connectedClients[clientSocket]->GetActiveDirectory());
	
	//Create new file function returns false if the file already exists
	if (FilesystemAPI::CreateNewFile(newPath) == false) {
		SV_WARN("Client, socket: {0}, couldn't create the file {1}, one with the same name already exists!", clientSocket, messageData);
		message_creator.CreateMessage(Action::NO_ACTION, static_cast<char>(ErrorCodes::INEXISTENT_DIRECTORY), "Couldn't create the directory, it already exists!");

		std::string message_client = message_creator.GetLastMessageAsString();
		if (connectedClients[clientSocket]->SupportsEncryption() == true)
			message_client = message_creator.EncryptMessage(connectedClients[clientSocket]->GetSecret());
		return message_client;
	}

	//The file was created
	SV_INFO("Client, socket: {0}, created file {1} in {2}", clientSocket, messageData, connectedClients[clientSocket]->GetActiveDirectory());

	message_creator.CreateMessage(Action::NO_ACTION, static_cast<char>(0), "Created the file!");
	std::string message_client = message_creator.GetLastMessageAsString();
	if (connectedClients[clientSocket]->SupportsEncryption() == true)
		message_client = message_creator.EncryptMessage(connectedClients[clientSocket]->GetSecret());
	return message_client;
}

const std::string RequestManager::ViewDirectoryContent(uint64_t clientSocket, const std::string& messageData)
{
	/*
	* Send back to the client all of the files and directories inside the current directory
	*/
	MessageCreator message_creator;

	if (messageData == "") {
		message_creator.CreateInvalidNumberOfTokensMessage();
		SV_WARN("Client, socket {0}, invalid number of arguments passed to create new directory", clientSocket);
		std::string message_client = message_creator.GetLastMessageAsString();
		if (connectedClients[clientSocket]->SupportsEncryption() == true)
			message_client = message_creator.EncryptMessage(connectedClients[clientSocket]->GetSecret());
		return message_client;
	}

	SV_INFO("Client, socket: {0}, requested the filenames of current directory {1}", clientSocket, connectedClients[clientSocket]->GetActiveDirectory());
	auto filenames = FilesystemAPI::ViewFilesInDirectory(connectedClients[clientSocket]->GetActiveDirectory());

	//If there are no files in the directory
	if (filenames.size() == 0) {
		SV_INFO("Client, socket {0}, there are no files in the current directory {1}", clientSocket, connectedClients[clientSocket]->GetActiveDirectory());
		message_creator.CreateMessage(Action::NO_ACTION, static_cast<char>(0), "There are 0 files in this directory!");
		std::string message_client = message_creator.GetLastMessageAsString();
		if (connectedClients[clientSocket]->SupportsEncryption() == true)
			message_client = message_creator.EncryptMessage(connectedClients[clientSocket]->GetSecret());
		return message_client;
	}

	//Put the files in the message and send the message back to the client
	SV_INFO("Client, socket {0}, {1} filenames have been sent to it", clientSocket, filenames.size());
	std::string message_data = "";

	//TO DO...Decide on the delimiter between arguments
	for (auto& details : filenames) {
		message_data += details.name + " " + details.type + " ";
	}

	message_creator.CreateMessage(Action::NO_ACTION, static_cast<char>(0), message_data);
	std::string message_client = message_creator.GetLastMessageAsString();
	if (connectedClients[clientSocket]->SupportsEncryption() == true)
		message_client = message_creator.EncryptMessage(connectedClients[clientSocket]->GetSecret());
	return message_client;
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
