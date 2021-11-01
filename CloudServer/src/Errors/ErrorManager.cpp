#include "cloudpch.h"
#include "ErrorManager.h"

#include "Core/Core.h"

std::string ErrorManager::errorsFilename = std::string();
std::map<int, Error*> ErrorManager::errors = std::map<int, Error*>();
bool ErrorManager::errorsInitialized = false;

void ErrorManager::Init(const std::string& filename)
{
    //Initializes the errors from the file
    std::ifstream in; in.open(filename);
    if (!in.is_open()) {
        errorsInitialized = false;
        SV_ERROR("Cannot open errors file: {0}", filename);
    }
    else {
        //The file has been found and opened
        errorsInitialized = true;
        //Read the supported error codes and details from the file and populate the map 
        while (!in.eof()) {
            std::string line;
            std::getline(in, line);
            size_t pos = line.find_first_of(" ");
            std::string codeStr = line.substr(0, pos);
            std::string details = line.substr(pos + 1);
            short code = static_cast<short>(atoi(codeStr.c_str()));
            errors[code] = new Error(code, details);
        }
    }
    in.close();
}

const Error& ErrorManager::GetError(int code)
{
    //Returns the error and it's details based on the error code
    return *errors[code];
}

const std::string ErrorManager::GetErrorDetails(const ErrorCodes& code)
{
    //Returns the error details string
    if (CONVERT_ERROR(code) < 255)
        return errors[static_cast<short>(code)]->GetErrorDetails();
    else
        return "Try again later!";
}

void ErrorManager::HandleError(const int& code)
{
    SV_WARN("ERROR: Code: {0}, Details: {1}", code, errors[code]->GetErrorDetails());
}

//Clears the errors from the error manager and the filename
void ErrorManager::DeleteErrorManager()
{
    errorsFilename.clear();

    for (auto& error : errors) {
        delete error.second;
    }
    errors.clear();
}
