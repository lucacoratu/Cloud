#ifndef MESSAGECREATOR_H
#define MESSAGECREATOR_H

#include "Message.h"

class MessageCreator {
private:
	static Message LastMessage;
public:
	static void CreateMessage(Action action, char errorNo, std::string data);
	static const std::string GetLastMessageAsString();
};

#endif
