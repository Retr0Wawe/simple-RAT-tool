# simple-RAT-tool v1.2
A simple tool for remote administration of your computer, implemented using dll and sockets.

### Start Server:
```cpp
#include "server.h"

int main()
{
	Server input("127.0.0.1", 4444); //ip:port
  
  	return 0;
}
```
### Setting up the entry point and building the Dll:
```cpp
#include "client_sock.h"

#define EXPORT _declspec(dllexport)

extern "C" EXPORT void client()
{
	Client client("127.0.0.1", 4444);
}
```
# Dll startup:
The dll can be launched in several ways:
### 1)Launch with rundll32.exe (during tests, the launch took place):
rundll32.exe name.dll, dll entry point

### 2)You can export dll from your program and run it there:
Coming soon :)
