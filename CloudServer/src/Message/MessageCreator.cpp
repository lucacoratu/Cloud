#include "cloudpch.h"
#include "MessageCreator.h"

#include "Core/Encryption/EncryptionAPI.h"

void MessageCreator::CreateWelcomeMessage()
{
    /*
    * Creates the welcome message for the newly connected client
    */
    std::string welcome_message = "Welcome from the cloud server!";
    LastMessage.SetHeader(static_cast<char>(Action::NO_ACTION), static_cast<char>(ErrorCodes::NO_ERROR_FOUND), static_cast<int>(welcome_message.size()));
    LastMessage.SetData(welcome_message);
}

void MessageCreator::CreatePublicKeyMessage(const std::string& publicKey)
{
    LastMessage.SetHeader(static_cast<char>(Action::RECEIVE_PUBLIC_KEY), static_cast<char>(ErrorCodes::NO_ERROR_FOUND), static_cast<int>(publicKey.size()));
    LastMessage.SetData(publicKey);
}

void MessageCreator::CreateReceivedPublicKey()
{
    /*
    * Creates a message that informs the client that its public key has been received
    */
    std::string received_message = "Public key has been received!";
    LastMessage.SetHeader(static_cast<char>(Action::NO_ACTION), static_cast<char>(ErrorCodes::NO_ERROR_FOUND), static_cast<int>(received_message.size()));
    LastMessage.SetData(received_message);
}

void MessageCreator::CreateRegisterCompletedMessage()
{
    /*
    * Creates a success message if the account registration has been successful 
    */
    std::string register_successful = "Your account has been registered successfully!";
    LastMessage.SetHeader(static_cast<char>(Action::NO_ACTION), static_cast<char>(ErrorCodes::NO_ERROR_FOUND), static_cast<int>(register_successful.size()));
    LastMessage.SetData(register_successful);
}

void MessageCreator::CreateRegisterFailedMessage(const ErrorCodes& code)
{
    /*
    * Creates a message with the afferent error codeand data field, if the register account fails
    */
    std::string message_data = ErrorManager::GetErrorDetails(code);
    LastMessage.SetHeader(static_cast<char>(Action::NO_ACTION), static_cast<char>(code), message_data.size());
    LastMessage.SetData(message_data);
}

void MessageCreator::CreateLoginFailedMessage()
{
    /*
    * Creates the message for when the login fails
    */
    std::string message_data = "Login failed, try again later!";
    LastMessage.SetHeader(static_cast<char>(Action::NO_ACTION), static_cast<char>(0), message_data.size());
    LastMessage.SetData(message_data);
}

void MessageCreator::CreateMessage(Action action, char errorNo, std::string data)
{
    /*
    * Creates a message based on the action, error number and data received in the parameters
    */
    LastMessage.SetHeader(static_cast<char>(action), errorNo, static_cast<int>(data.size()));
    LastMessage.SetData(data);
}

std::string MessageCreator::EncryptMessage(std::string key)
{
    return EncryptionAPI::Encrypt(this->LastMessage.GetMessageAsString(), key);
}

const std::string MessageCreator::GetLastMessageAsString()
{
    /*
    * Returns the LastMessage variable as a byte array 
    */
    return LastMessage.GetMessageAsString();
}
