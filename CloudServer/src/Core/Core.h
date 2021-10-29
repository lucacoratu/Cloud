#ifndef CORE_H
#define CORE_H

#define _CRT_SECURE_NO_WARNINGS

//If the windows platform is detected then use the Winsock32 header for the server else use the linux alternative
#ifdef SV_WINDOWS
	#include <WS2tcpip.h>
	#pragma comment (lib, "ws2_32.lib")

	#define _WINSOCK_DEPRECATED_NO_WARNINGS
#else
	#ifdef SV_LINUX
	//Add the library used for the linux sockets
		#include ...
	#endif
#endif


#endif // !CORE_H

