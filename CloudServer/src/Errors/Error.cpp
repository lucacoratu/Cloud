#include "cloudpch.h"
#include "Error.h"

Error::Error(const short& code, const std::string& details)
{
}

Error::~Error()
{
}

const std::string& Error::GetErrorDetails() const
{
    //Returns the details string of the error
    return this->details;
}

const short& Error::GetErrorCode() const
{
    //Returns the error code
    return this->code;
}
