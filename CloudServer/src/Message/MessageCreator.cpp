#include "cloudpch.h"
#include "MessageCreator.h"

#include "Errors/ErrorCodes.h"

Message MessageCreator::LastMessage = Message();

void MessageCreator::CreateWelcomeMessage()
{
    /*
    * Creates the welcome message for the newly connected client
    */
    std::string welcome_message = "Welcome from the cloud server!";
    LastMessage.SetHeader(static_cast<char>(Action::NO_ACTION), static_cast<char>(ErrorCodes::NO_ERROR_FOUND), static_cast<int>(welcome_message.size()));
    LastMessage.SetData(welcome_message);
}

void MessageCreator::CreateMessage(Action action, char errorNo, std::string data)
{
    /*
    * Creates a message based on the action, error number and data received in the parameters
    */
    LastMessage.SetHeader(static_cast<char>(action), errorNo, static_cast<int>(data.size()));
    LastMessage.SetData(data);
}

const std::string MessageCreator::GetLastMessageAsString()
{
    /*
    * Returns the LastMessage variable as a byte array 
    */
    return LastMessage.GetMessageAsString();
}
