#include "cloudpch.h"
#include "MessageParser.h"

//Empty message
Message MessageParser::LastMessage = Message();
std::vector<std::string> MessageParser::MessageTokens = std::vector<std::string>();

static union { char c[4]; unsigned long mylong; } endian_test = { { 'l', '?', '?', 'b' } };
#define ENDIANNESS ((char)endian_test.mylong)


void MessageParser::CreateMessageFromString(const std::string& data)
{
	//Creates a message from a string given and stores it in the LastMessage static variable
	//Takes the header and puts it in the LastMessage header
	MessageHeader header = { 0, 0, 0 };
	header.action = data[0];
	header.errorNo = data[1];
	//Convert the 4 bytes starting from data[2] into an integer
	std::string dataLength = data.substr(2, 4);
	
	std::string dataLengthLittle = "";
	int* aux = NULL;

	if (ENDIANNESS == 'l') { /* little endian */
		//printf("Little endian");
		for (int i = 3; i >= 0; i--) {
			dataLengthLittle += dataLength[i];
		}
		aux = (int*)dataLengthLittle.c_str();
	}

	if (ENDIANNESS == 'b') { /* big endian */
		aux = (int*)dataLength.c_str();
	}

	header.dataLength = *aux;
	std::string dataStr = data.substr(6, header.dataLength);

	//Set the last message
	SetLastMessage(header, dataStr);
}

void MessageParser::SetLastMessage(const Message& message)
{
	//Copies the message received in the parameter into the LastMessage static member variable
	LastMessage = message;
}

void MessageParser::SetLastMessage(const MessageHeader& header, const std::string& data)
{
	//Sets the static member variable LastMessage by assembling the header and the data
	LastMessage.SetHeader(header);
	LastMessage.SetData(data);
}

const Action& MessageParser::GetMessageAction()
{
	//Converts the Action field in the message header to an enum which lists the supported actions
	return static_cast<Action>(LastMessage.GetMessageHeader().action);
}

const std::string& MessageParser::GetMessageData()
{
	//Returns the data string from the message (message without the header)
	return LastMessage.GetData();
}

const std::vector<std::string>& MessageParser::GetMessageTokens(const char delim)
{
	//Clear the message tokens
	MessageTokens.clear();
	//Breaks the LastMessage data into tokens
	size_t count = 0;
	MessageTokens.push_back("");
	for (auto ch : LastMessage.GetData()) {
		if (ch == delim) {
			MessageTokens.push_back(std::string());
			count++;
		}
		MessageTokens[count] += ch;
	}
	return MessageTokens;
}
