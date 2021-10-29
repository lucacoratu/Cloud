#ifndef MESSAGE_H
#define MESSAGE_H

#include "Action.h"

//Header of the message
struct MessageHeader {
	char action;
	//An error code that can be used by the client to display a certain
	char errorNo;
	int dataLegth;
};

//Message Header: | action(1 bytes) | errorNo (1 byte) | dataLength(4 bytes) | data (dataLength bytes) |  

//The full message
class Message {
private:
	MessageHeader header;
	std::string data;
public:
	//Constrcuctors / Destructors
	Message();
	Message(const MessageHeader& header, const std::string& data);
	Message(const Message& other);
	~Message();

	//Functions
	//Debug: print the message
	//void DebugMessagePrint();
	
	//Accessors
	const std::string& GetData() const;
	const MessageHeader& GetMessageHeader() const;
	const std::string GetMessageAsString() const;

	//Modifiers
	void SetHeader(const MessageHeader& header);
	void SetHeader(const char& action, const char& errorNo, int dataLength);
	void SetData(const std::string& data);

	//Operators
	Message& operator=(const Message& other);
};


#endif
