#include "cloudpch.h"
#include "TCPListener.h"

#include "Core/Log.h"

#include <mutex>
#include <condition_variable>
#include <thread>

struct ThreadMessage {
	std::string message = "";
	uint64_t clientSocket = 0;
};

//Thread pool
static std::thread thread_pool[4];

//Mutex for the message queue
static std::mutex mutex;

//Condition variable
static std::condition_variable condition_var;

//The queue of messages
static std::queue<ThreadMessage> message_queue;

//Thread functions
void TcpListener::SubmitMessage(uint64_t clientSocket, std::string message) {
	//Producer thread
	//When a client sends a message to the server the main thread (only one who can submit a message)
	//Creates a task for the consumers

	//Lock the message_queue
	std::lock_guard<std::mutex> lock(mutex);
	ThreadMessage message_thread = { message, clientSocket };
	message_queue.push(message_thread);
	condition_var.notify_one();
	//Unlock the message queue (the mutex unlocks when the scope ends -- lock guard is scope based)
}

void TcpListener::ExecuteThread() {
	/*
	*Take a message from the message queue
	*If there are any messages in the queue
	*If there are no messages in the queue then wait for messages to appear
	*/
	while (1) {
		ThreadMessage message;
		int found = 0;

		{
			//The lock is unique to this scope
			std::unique_lock<std::mutex> lock(mutex);
			condition_var.wait(lock, [&]() {
				//Waits until the message queue is not empty 
				return !message_queue.empty();
				});

			if (!message_queue.empty()) {
				message = message_queue.front();
				found = 1;
				message_queue.pop();
			}
			//Mutex unlocks here
		}

		if (found == 1)
			//The thread got a message to work on
			onMessageReceived(message.clientSocket, message.message, static_cast<int>(message.message.size()));
	}

}

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

	//Inititalize the thread pool
	for (int i = 0; i < 4; i++) {
		thread_pool[i] = std::thread([&]() {
			ExecuteThread();
			});
	}

	return 0;
}

int TcpListener::run()
{
	bool running = true;

	SV_INFO("Server is starting...");

	while (running)
	{

		fd_set copy = master;

		// See who's talking to us
		int socketCount;
		socketCount = select(0, &copy, nullptr, nullptr, nullptr);


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
				SOCKET client = accept(s_socket, (sockaddr*)&client_info, p);

				//char* connected_ip = inet_ntoa(client_info.sin_addr);
				//int port = ntohs(client_info.sin_port);
				//std::stringstream ss; ss << connected_ip << ":" << port;

				// Add the new connection to the list of connected clients
				FD_SET(client, &master);

				onClientConnected(client);
			}
			else // It's an inbound message
			{
				const unsigned int MAX_BUF_LENGTH = 32768;
				std::vector<char> buffer(MAX_BUF_LENGTH);

				// Receive message
				int bytesIn = recv(sock, &buffer[0], static_cast<int>(buffer.size()), 0);
				if (bytesIn <= 0)
				{
					// Drop the client
					onClientDisconnected(sock);
					closesocket(sock);
					FD_CLR(sock, &this->master);
				}
				//else if (bytesIn == MAX_BUF_LENGTH) {
				//	std::vector<char> buffer(MAX_BUF_LENGTH);
				//	std::string rcv;
				//	int bytesReceived = 0;

				//	do {
				//		bytesReceived = recv(sock, &buffer[0], static_cast<int>(buffer.size()), 0);
				//		// append string from buffer.
				//		if (bytesReceived <= 0) {
				//			// Drop the client
				//			onClientDisconnected(sock);
				//			closesocket(sock);
				//			FD_CLR(sock, &this->master);
				//		}
				//		else {
				//			rcv.append(buffer.cbegin(), buffer.cend());
				//		}
				//	} while (bytesReceived == MAX_BUF_LENGTH);


				//	onMessageReceived(sock, rcv, static_cast<int>(rcv.size()));
				//	rcv.clear();
				//	return;
				//}
				else {
					std::string rcv;
					rcv.append(buffer.cbegin(), buffer.cend());

					this->SubmitMessage(sock, rcv);

					//onMessageReceived(sock, rcv, static_cast<int>(rcv.size()));
					rcv.clear();
					buffer.clear();
				}
			}
		}

	}

	//Close the database connection
	DatabaseAPI::CloseDatabaseConnection();

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

//Send the message to the client
void TcpListener::sendToClient(uint64_t clientSocket, const char* msg, size_t length)
{
	send(clientSocket, msg, static_cast<int>(length), 0);
}

void TcpListener::sendToClient(uint64_t clientSocket, std::string& msg, size_t length) {
	send(clientSocket, &msg[0], static_cast<int>(length), 0);
}


void TcpListener::broadcastToClients(uint64_t sendingClient, const char* msg, int length) {

}

void TcpListener::onClientConnected(uint64_t clientSocket)
{

}

void TcpListener::onClientDisconnected(uint64_t clientSocket)
{

}

void TcpListener::onMessageReceived(uint64_t clientSocket, std::string& msg, int length)
{

}
