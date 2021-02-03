#pragma once
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable: 4996)
#include <iostream>
#include <winsock2.h>
#include <string>

class Server {
	const char* IP;
	const short PORT;
	SOCKADDR_IN addr;
	int size_addr = sizeof(addr);
	static const short max_connections = 50;
	SOCKET Users[max_connections];
	short con_counts = 0;
public:
	Server(const char* ip, const short port) : IP(ip), PORT(port) {
		addr.sin_addr.s_addr = inet_addr(IP);
		addr.sin_port = htons(PORT);
		addr.sin_family = AF_INET;
		sock_start();
	}

	void client_handler(int user_id)
	{
		std::string message;
		int recv_result, send_result;
		const int size_buffer = 216;
		char* buffer = new char[size_buffer + 1]();
		while (true) {
			std::cout << "\n>> ";
			std::getline(std::cin, message);

			send_result = send(Users[user_id], message.c_str(), message.length(), 0);
			recv_result = recv(Users[user_id], buffer, size_buffer, 0);
			buffer[recv_result] = '\0';

			if(recv_result < 0){
				std::cout << "Data acceptance error!" << std::endl;
			}

			std::cout << "\n>> " << buffer << std::endl;
			memset(buffer, 0, size_buffer);
		}
	}

	void ping_client()
	{
		//coming soon :)
	}

	void connection_accept(SOCKET sock)
	{
		for (int i = 0; i < max_connections; i++) {
			SOCKET user_connection = accept(sock, (SOCKADDR*)&addr, &size_addr);

			if (user_connection == INVALID_SOCKET) {
				std::cout << "\nAccept error: " << WSAGetLastError() << std::endl;
			}

			Users[i] = user_connection; con_counts++;
			std::cout << "\nSucces: " << i << std::endl;

			while (con_counts <= max_connections) {
				client_handler(i);
			}
		}
	}
	void sock_start()
	{
		WSAData wsaData;
		WORD DLLVersion = MAKEWORD(2, 1);

		if (WSAStartup(DLLVersion, &wsaData) != 0) {
			std::cout << "\nVersion check error!" << std::endl;
			exit(1);
		}
		std::cout << "The version check was successful!" << std::endl;

		SOCKET set_sock = socket(AF_INET, SOCK_STREAM, NULL);

		if (set_sock == INVALID_SOCKET) {
			std::cout << "\nSocket error: " << WSAGetLastError() << std::endl;
			system("pause");
			exit(1);
		}
		std::cout << "\nSocket installation was successful!" << std::endl;

		if (bind(set_sock, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {
			std::cout << "\nBind failed with error: " << WSAGetLastError() << std::endl;
			system("pause");
			exit(1);
		}
		std::cout << "\nBinding the socket to the address was successful!" << std::endl;

		if (listen(set_sock, SOMAXCONN) == INVALID_SOCKET) {
			std::cout << "\nStart listening error: " << WSAGetLastError() << std::endl;
			system("pause");
			exit(1);
		}
		std::cout << "\nStart listening successfully!" << std::endl;

		std::cout << "\nThe server started at: " << IP << ":" << PORT << std::endl;

		while (con_counts <= max_connections) {
			connection_accept(set_sock);
		}
	}
};
