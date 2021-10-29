#include "cloudpch.h"
#include "Message.h"

//Constructors / Destructors
Message::Message()
	: header({ static_cast<char>(Action::NO_ACTION), 0 , 0}), data("")
{

}

Message::Message(const MessageHeader& header, const std::string& data)
	: header(header), data(data)
{

}

Message::Message(const Message& other)
	: header(other.header), data(other.data)
{

}

Message::~Message()
{

}


//Functions

//Accessors
const std::string& Message::GetData() const
{
	return this->data;
}

const MessageHeader& Message::GetMessageHeader() const
{
	return this->header;
}

const std::string Message::GetMessageAsString() const
{
	std::string message;
	//Convert the header into bytes array and put the bytes in the string
	message += static_cast<char>(this->header.action);
	message += static_cast<char>(this->header.errorNo);
	//Convert the 4 bytes from the dataLength field into byte array
	char* ptrDataLength = (char*)(&(this->header.dataLegth));
	for (int i = 0; i < 4; i++)
		message += ptrDataLength + i;
	message += this->data;

	return message;
}

void Message::SetHeader(const MessageHeader& header)
{
	this->header = header;
}

void Message::SetHeader(const char& action, const char& errorNo, int dataLength)
{
	this->header = { action, errorNo, dataLength };
}

void Message::SetData(const std::string& data)
{
	this->data = data;
}

Message& Message::operator=(const Message& other)
{
	this->header = other.header;
	this->data = other.data;
	return *this;
}
