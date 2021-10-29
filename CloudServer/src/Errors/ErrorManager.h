#ifndef ERRORMANAGER_H
#define ERRORMAANGER_H

#include "Error.h"

class ErrorManager {
private:
	//Static member variables
	static std::string errorsFilename;
	static std::map<short, Error*> errors;
	static bool errorsInitialized;
public:
	//Functions

	//Static Functions
	static void Init(const std::string& filename);
	static const Error& GetError(short code);
	static void HandleError(const short& code);

	//Clear function
	static void DeleteErrorManager();
};



#endif
