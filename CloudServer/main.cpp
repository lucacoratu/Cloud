#include "CloudServer.h"

int main() {
	CloudServer& server = CloudServer::createInstance("127.0.0.1", 54000);
	server.init();

	server.run();
	
	server.deleteInstance();
	return 0;
}