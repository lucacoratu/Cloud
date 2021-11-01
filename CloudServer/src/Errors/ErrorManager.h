#ifndef ERRORMANAGER_H
#define ERRORMAANGER_H

#include "Error.h"

class ErrorManager {
private:
	//Static member variables
	static std::string errorsFilename;
	static std::map<int, Error*> errors;
	static bool errorsInitialized;
public:
	//Functions

	//Static Functions
	static void Init(const std::string& filename);
	static const Error& GetError(int code);
	static const std::string GetErrorDetails(const ErrorCodes& code);
	static void HandleError(const int& code);

	//Clear function
	static void DeleteErrorManager();
};



#endif
