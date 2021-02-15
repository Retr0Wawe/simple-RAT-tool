#pragma once
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable: 4996)
#include <iostream>
#include <winsock2.h>
#include <string>

class Server {
	const char* IP;
	const short PORT;
	static const short max_connections = 50;
	SOCKADDR_IN addr;
	SOCKET set_sock;
	SOCKET users[max_connections];
	int size_addr = sizeof(addr);
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
		const int size_buffer = 256;
		int recv_result = 0, send_result = 0;
		std::string message;
	
		while (set_sock != -1) {
			char* buffer = new char[size_buffer + 1]();
			buffer[size_buffer] = '\0';

			if (recv_result < 0) {
				std::cout << "Data sending error!" << std::endl;
				if (ping_client() > 0) {
					return;
				}
				continue;
			}
			std::cout << "\n>> ";
			std::getline(std::cin, message);

			command_handler(message, user_id);
			send(users[user_id], message.c_str(), message.length(), 0);

			recv_result = recv(users[user_id], buffer, size_buffer, 0);
			std::cout << std::endl << buffer << std::endl;	

			delete[] buffer;
		}
	}

	void command_handler(std::string data, int user_id)
	{
		if (strcmp(data.c_str(), "help") == 0) {
			std::cout << "\nAll commands: " << std::endl <<
				"---------------------\n" << std::endl <<
				"1)get_pdir [name.exe] " << "- List the current directory of the given process.\n" << std::endl <<
				"2)msg [text] " << "- Display a message on the screen as a MessageBox.\n" << std::endl <<
				"3)pwd " << "- Get the current directory of the file location\n" << std::endl <<
				"4)disc " << "- Disconnecting the client from the server\n" << std::endl <<
				"5)clear " << "- Clear screen\n" << std::endl <<
				"5)exit " << "- Close server\n" << std::endl <<
				"---------------------";
		}

		if (strcmp(data.c_str(), "clear") == 0) {
			system("cls");
		}

		if(strcmp(data.c_str(), "exit") == 0) {
			exit(0);
		}
	}
	 
	int ping_client()
	{
		int recv_result[max_connections];
		char tmp[max_connections + 1];
		int count = 0;

		for (int i = 0; i < max_connections; i++) {					//there is a preparation for streams, but you still need to live before that :)
			recv_result[i] = recv(users[i], tmp, 1, 0);

			if(recv_result[i] < 0){
				std::cout << "\nClient not connected. Waiting for client connection..." << std::endl;
				connection_accept(set_sock);
			}
			
			if (recv_result[i] > 0) {
				count++;
			}
			return count;
		}
	}

	void connection_accept(SOCKET sock)
	{
		for (int i = 0; i < max_connections; i++) {
			SOCKET user_connection = accept(sock, (SOCKADDR*)&addr, &size_addr);

			if (user_connection == INVALID_SOCKET) {
				std::cout << "\nAccept error: " << WSAGetLastError() << std::endl;
			}

			users[i] = user_connection; con_counts++;
			std::cout << "\nSucces connection: " << i << std::endl;

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

		set_sock = socket(AF_INET, SOCK_STREAM, NULL);

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

		std::cout << std::endl << "To find out the list of commands write - help" << std::endl;

		while (con_counts <= max_connections) {
			connection_accept(set_sock);
		}
	}
};