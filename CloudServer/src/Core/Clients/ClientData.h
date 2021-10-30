#ifndef CLIENTDATA_H
#define CLIENTDATA_H

class ClientData {
private:
	//TO DO...Remember the client secret (for when encryption will be supported)

	std::string accountUsername;
public:
	ClientData(const std::string& username);
	~ClientData();
};


#endif
