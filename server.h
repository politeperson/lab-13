#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#define _WIN32_WINNT 0x501

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>


#pragma comment (lib, "Ws2_32->lib")//un comentario pragma se usa para 
									//decirle al linker que usareos esa
									//libreria dinamica

class My_Server{//creo mi clase servidor
	private:
		#define DEFAULT_PORT "27015"//variables por defecto
		int DEFAULT_BUFLEN; 
		int iResult;
		int iSendResult;

		//componentes de la libreria winsock
		WSADATA wsaData;
		class addrinfo *result = NULL;
    	class addrinfo hints;
		SOCKET ListenSocket = INVALID_SOCKET;//socket para escuchar al cliente
    	SOCKET ClientSocket = INVALID_SOCKET;//socket del cliente

    public://constrctor de la clase y sus funciones
    	My_Server(const int _DEFAULT_BUFLEN){}
    	int init_Components(){}
    	int connect_port(){}
    	int create_socket(){}
    	int listen_socket(){}
    	int accept_client(){}
    	void clean_buf(char message[]){}
    	void print_buf(char message[]){}
    	int receive_and_echo_message(){}
    	int shutdown_server(){}
    	void cleanup_server(){}
};