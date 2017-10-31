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

#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

using namespace std;
class My_Client{//creo mi clase cliente
	private:
		#define DEFAULT_PORT "27015"//variables por defecto
		int DEFAULT_BUFLEN; 
		int iResult;
		int iSendResult;

		//componentes de la libreria winsock
		WSADATA wsaData;
        class addrinfo *ptr = NULL;
		class addrinfo *result = NULL;
    	class addrinfo hints;
		SOCKET ConnectSocket = INVALID_SOCKET;//socket para escuchar al cliente

    public://constrctor de la clase y sus funciones
    	My_Client(const int _DEFAULT_BUFLEN);
    	int init_Components();
    	int connect_port();
    	int create_socket();
    	int connect_server();
    	int send_and_receive_message();
    	int shutdown_client();
};