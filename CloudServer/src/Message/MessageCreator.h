#ifndef MESSAGECREATOR_H
#define MESSAGECREATOR_H

#include "Message.h"
#include "Errors/ErrorManager.h"

class MessageCreator {
private:
	Message LastMessage;
public:
	//Constructors / Destructors
	MessageCreator();
	~MessageCreator();

	//Functions
	void CreateWelcomeMessage();
	void CreatePublicKeyMessage(const std::string& publicKey);
	void CreateReceivedPublicKey();
	void CreateRegisterCompletedMessage();
	void CreateRegisterFailedMessage(const ErrorCodes& code);
	void CreateLoginFailedMessage();
	void CreateInvalidNumberOfTokensMessage();
	void CreateMessage(Action action, char errorNo, const std::string& data);
	std::string EncryptMessage(std::string key);
	const std::string GetLastMessageAsString();
};

#endif
