#include "client_sock.h"

#define EXPORT _declspec(dllexport)

extern "C" EXPORT void client()
{
	Client client("127.0.0.1", 4444);
}
