#ifndef CLOUDSERVER_H
#define CLOUDSERVER_H

#include "TCPListener.h"

class CloudServer : public TcpListener
{
private:
	//Server instance
	static CloudServer* instance;

	//Constructors / Destructors
	CloudServer(const char* ipAddress, int port) :
		TcpListener(ipAddress, port) {
	}

	~CloudServer() { }
protected:

	// Handler for a new connection
	void onClientConnected(uint32_t clientSocket) override final;

	// Handler for a client disconnection
	void onClientDisconnected(uint32_t clientSocket) override final;

	// Handler for a message received from a client
	void onMessageReceived(uint32_t clientSocket, std::string& msg, int length) override final;
public:
	//Instance functions
	static CloudServer& createInstance(const char* ipAdress, int port);
	static void deleteInstance();
};


#endif