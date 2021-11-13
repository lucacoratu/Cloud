#ifndef MESSAGEPARSER_H
#define MESSAGEPARSER_H

#include "Message.h"

//Parses the message received from the client
class MessageParser {
private:
	//Memeber variables
	Message LastMessage;
	std::vector<std::string> MessageTokens;
public:
	//Constructors / Destructors
	MessageParser();
	~MessageParser();

	//Functions
	void CreateMessageFromString(const std::string& data);
	void SetLastMessage(const Message& message);
	void SetLastMessage(const MessageHeader& header, const std::string& data);
	const Action GetMessageAction();
	const std::string& GetMessageData();
	const std::vector<std::string>& GetMessageTokens(const char delim = ' ');
	const std::string DecryptMessage(const std::string& data, const std::string key);
};


#endif
