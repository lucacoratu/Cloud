#ifndef MESSAGEPARSER_H
#define MESSAGEPARSER_H

#include "Message.h"

//Parses the message received from the client
class MessageParser {
private:
	//Memeber variables
	static Message LastMessage;
	static std::vector<std::string> MessageTokens;
public:
	//Functions
	static void CreateMessageFromString(const std::string& data);
	static void SetLastMessage(const Message& message);
	static void SetLastMessage(const MessageHeader& header, const std::string& data);
	static const Action& GetMessageAction();
	static const std::string& GetMessageData();
	static const std::vector<std::string>& GetMessageTokens(const char delim = ' ');
};


#endif
