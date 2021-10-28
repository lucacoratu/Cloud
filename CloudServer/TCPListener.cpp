#include "cloudpch.h"
#include "TCPListener.h"

#include "Log.h"

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

int TcpListener::init()
{
	SV_INFO("Server is initializing...");

	// Initialze winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0)
	{
		return wsOk;
	}

	// Create a socket
	s_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (s_socket == INVALID_SOCKET)
	{
		SV_ERROR("Could not create the socket!, {0}", WSAGetLastError());
		return WSAGetLastError();
	}

	// Bind the ip address and port to a socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(s_port);
	inet_pton(AF_INET, s_ipAddress, &hint.sin_addr);

	if (bind(s_socket, (sockaddr*)&hint, sizeof(hint)) == SOCKET_ERROR)
	{
		SV_ERROR("Could not bind the ip address and port to the socket! {0}", WSAGetLastError());
		return WSAGetLastError();
	}

	// Tell Winsock the socket is for listening 
	if (listen(s_socket, SOMAXCONN) == SOCKET_ERROR)
	{
		return WSAGetLastError();
	}

	SV_INFO("Listening socket sucessfully initialized!");

	// Create the master file descriptor set and zero it
	FD_ZERO(&master);

	// Add our first socket that we're interested in interacting with; the listening socket!
	// It's important that this socket is added for our server or else we won't 'hear' incoming
	// connections 
	FD_SET(s_socket, &master);

	return 0;
}

int TcpListener::run()
{
	bool running = true;

	//std::cout << "Server is running...\r\n";
	SV_INFO("Server is starting...");

	while (running)
	{

		fd_set copy = master;

		// See who's talking to us
		int socketCount;
		std::thread th1([&socketCount, &copy]() {
			socketCount = select(0, &copy, nullptr, nullptr, nullptr);
			});
		th1.join();


		// Loop through all the current connections / potential connect
		for (int i = 0; i < socketCount; i++)
		{
			// Makes things easy for us doing this assignment
			SOCKET sock = copy.fd_array[i];

			//Adresa IP a clientului
			struct sockaddr_in client_info = { 0 };
			int* p = new int;
			//Dimeniunea adresei (IPv4)
			*p = INET_ADDRSTRLEN;

			// Is it an inbound communication?
			if (sock == s_socket)
			{
				// Accept a new connection
				std::thread th2([&]() {
					SOCKET client = accept(s_socket, (sockaddr*)&client_info, p);

					//char* connected_ip = inet_ntoa(client_info.sin_addr);
					//int port = ntohs(client_info.sin_port);
					//std::stringstream ss; ss << connected_ip << ":" << port;

					// Add the new connection to the list of connected clients
					FD_SET(client, &master);

					onClientConnected(client);
					});

				//th2.join();
			}
			else // It's an inbound message
			{
				std::thread th3([&]() {
					char buf[4096];
					ZeroMemory(buf, 4096);

					// Receive message
					int bytesIn = recv(sock, buf, 4096, 0);
					if (bytesIn <= 0)
					{
						// Drop the client
						onClientDisconnected(sock);
						closesocket(sock);
						FD_CLR(sock, &this->master);
					}
					else {
						const unsigned int MAX_BUF_LENGTH = 1024;
						std::vector<char> buffer(MAX_BUF_LENGTH);
						std::string rcv;
						int bytesReceived = 0;

						do {
							bytesReceived = recv(sock, &buffer[0], static_cast<int>(buffer.size()), MSG_WAITALL);
							// append string from buffer.
							if (bytesReceived <= 0) {
								// Drop the client
								onClientDisconnected(sock);
								closesocket(sock);
								FD_CLR(sock, &this->master);
							}
							else {
								rcv.append(buffer.cbegin(), buffer.cend());
							}
						} while (bytesReceived == MAX_BUF_LENGTH);


						onMessageReceived(sock, rcv, static_cast<int>(rcv.size()));
						rcv.clear();
						return;
					}

				});

				th3.join();
			}
		}

	}

	// Remove the listening socket from the master file descriptor set and close it
	// to prevent anyone else trying to connect.
	FD_CLR(s_socket, &master);
	closesocket(s_socket);

	while (master.fd_count > 0)
	{
		// Get the socket number
		SOCKET sock = master.fd_array[0];

		// Remove it from the master file list and close the socket
		FD_CLR(sock, &master);
		closesocket(sock);
	}

	// Cleanup winsock
	WSACleanup();
	return 0;
}

//Trimite mesaj unui utilizator pe baza socket-ului
void TcpListener::sendToClient(uint32_t clientSocket, const char* msg, size_t length)
{
	send(clientSocket, msg, static_cast<int>(length), 0);
}

void TcpListener::sendToClient(uint32_t clientSocket, std::string& msg, size_t length) {
	send(clientSocket, &msg[0], static_cast<int>(length), 0);
}


void TcpListener::broadcastToClients(uint32_t sendingClient, const char* msg, int length) {

}

void TcpListener::onClientConnected(uint32_t clientSocket)
{

}

void TcpListener::onClientDisconnected(uint32_t clientSocket)
{

}

void TcpListener::onMessageReceived(uint32_t clientSocket, std::string& msg, int length)
{

}
