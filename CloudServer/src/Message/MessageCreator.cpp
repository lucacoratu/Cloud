#include "cloudpch.h"
#include "MessageCreator.h"


Message MessageCreator::LastMessage = Message();

void MessageCreator::CreateMessage(Action action, char errorNo, std::string data)
{
}

const std::string MessageCreator::GetLastMessageAsString()
{
    return std::string();
}
