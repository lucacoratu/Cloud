#ifndef MESSAGECREATOR_H
#define MESSAGECREATOR_H

#include "Message.h"
#include "Errors/ErrorManager.h"

class MessageCreator {
private:
	static Message LastMessage;
public:
	static void CreateWelcomeMessage();
	static void CreateRegisterCompletedMessage();
	static void CreateRegisterFailedMessage(const ErrorCodes& code);
	static void CreateLoginFailedMessage();
	static void CreateMessage(Action action, char errorNo, std::string data);
	static const std::string GetLastMessageAsString();
};

#endif
