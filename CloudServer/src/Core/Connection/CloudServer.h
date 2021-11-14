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

	~CloudServer() { /*Clear all of the data */ }
protected:

	// Handler for a new connection
	void onClientConnected(uint64_t clientSocket) override final;

	// Handler for a client disconnection
	void onClientDisconnected(uint64_t clientSocket) override final;

	// Handler for a message received from a client
	void onMessageReceived(uint64_t clientSocket, std::string& msg, int length) override final;
public:
	//Instance functions
	static CloudServer& createInstance(const char* ipAdress, int port);
	static void deleteInstance();
};


#endif