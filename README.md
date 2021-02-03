# simple-RAT-tool
A simple tool for remote administration of your computer, implemented using dll and sockets.
#Start Server:
```cpp
#include "server.h"

int main()
{
	Server input("127.0.0.1", 4444); //ip:port
  
  return 0;
}
```
#Assembly DLL:
```cpp
#include "client_sock.h"

#define EXPORT _declspec(dllexport)

extern "C" EXPORT void client()
{
	Client client("127.0.0.1", 4444);
}

