#include "client_sock.h"

#define EXPORT _declspec(dllexport)

extern "C" EXPORT void client()
{
	Client* start = new Client("127.0.0.1", 4444);
	CreateThread(0, 0, LPTHREAD_START_ROUTINE(start->connect_client()), 0, 0, 0);
}