#ifndef TCPLISTENER_H
#define TCPLISTENER_H

#include "Core/Log.h"
#include "Core/Core.h"

#include "Core/Database/DatabaseAPI.h"

class TcpListener
{

public:
	TcpListener(const char* ipAddress, int port) :
		s_ipAddress(ipAddress), s_port(port), s_socket(0) {
		//Initialize the log class
		Log::init();

		//Inititalize the database
		DatabaseAPI::InitDatabase("./Database/cloud_database.db");

		//Inititalize the member variables
		this->master = { 0 };
	}

	virtual ~TcpListener() {  }

	// Initialize the listener
	int init();

	// Run the listener
	int run();

protected:
	// Handler for a new connections
	virtual void onClientConnected(uint32_t clientSocket);

	// Handler for disconnecting 
	virtual void onClientDisconnected(uint32_t clientSocket);

	// Handler pentru un mesaj primit de la client
	virtual void onMessageReceived(uint32_t clientSocket, std::string& msg, int length);

	// Sending message to a client
	void sendToClient(uint32_t clientSocket, const char* msg, size_t length);

	//Sending message to a client
	void sendToClient(uint32_t clientSocket, std::string& msg, size_t length);

	//Sending message to all clients
	void broadcastToClients(uint32_t sendingClient, const char* msg, int length);
private:

	const char* s_ipAddress;					// Adresa IP a serverului 
	int				s_port;						// Nr. portului
	uint32_t		s_socket;					// Internal FD for the listening socket
	fd_set			master;						// Master file descriptor set

protected:
	//Variables
};


#endif