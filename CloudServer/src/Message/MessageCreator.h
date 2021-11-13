#ifndef MESSAGECREATOR_H
#define MESSAGECREATOR_H

#include "Message.h"
#include "Errors/ErrorManager.h"

class MessageCreator {
private:
	Message LastMessage;
public:
	void CreateWelcomeMessage();
	void CreatePublicKeyMessage(const std::string& publicKey);
	void CreateReceivedPublicKey();
	void CreateRegisterCompletedMessage();
	void CreateRegisterFailedMessage(const ErrorCodes& code);
	void CreateLoginFailedMessage();
	void CreateMessage(Action action, char errorNo, std::string data);
	std::string EncryptMessage(std::string key);
	const std::string GetLastMessageAsString();
};

#endif
