#include "cloudpch.h"
#include "Core/Connection/CloudServer.h"

int main() {
	CloudServer& server = CloudServer::createInstance("127.0.0.1", 54000);
	server.init();

	//comentariu de test

	server.run();
	
	server.deleteInstance();
	return 0;
}