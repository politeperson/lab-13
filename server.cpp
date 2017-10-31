#include <iostream>
#include "server.h"

using namespace std;

My_Server::My_Server(const int _DEFAULT_BUFLEN){
    DEFAULT_BUFLEN = _DEFAULT_BUFLEN;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;
}
int My_Server::init_Components(){
    // Iniciar los componentes de Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        cout << "error al iniciar el programa: " << iResult << endl;
        return 1;
    }
}

int My_Server::connect_port(){
    // añadir el la direccion del servidor y el puerto de conección
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        cout << "la funcion getaddrinfo fallo, error: " << iResult << endl;
        WSACleanup();
        return 1;
    }
}

int My_Server::create_socket(){
    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        cout << "el socket fallo, error: " << WSAGetLastError() << endl;
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }
}

int My_Server::listen_socket(){
    // iniciar la escucha al cliene mediante TCP
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        cout << "conección fallida, error: " << WSAGetLastError() << endl;
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);//liberar el espacio de result

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        cout << "escucha fallida, error: " << WSAGetLastError() << endl;
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
}

int My_Server::accept_client(){
    // Aceptando el socket del cliente
    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
        cout << "accept failed with error: "<< WSAGetLastError() << endl;
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    // ya no necesitamos el socket del server asi que lo limpiamos
    closesocket(ListenSocket);
}

void My_Server::clean_buf(char message[]){
    for(int i = 0; i < DEFAULT_BUFLEN; i++){
        message[i] = '\0';
    }
}

void My_Server::print_buf(char message[]){
    int i = 0;
    while(message[i] != '\0' && i < DEFAULT_BUFLEN){
        cout << message[i];
    }
    cout << endl;
}
int My_Server::receive_and_echo_message(){
    char message[DEFAULT_BUFLEN];
    // recibir y enviar mensaje al cliente
    do{
        iResult = recv(ClientSocket, message, DEFAULT_BUFLEN, 0);
        if (iResult > 0) {
            cout << "cliente: ";
            print_buf(message);

            // Echo the buffer back to the sender
            iSendResult = send( ClientSocket, message, iResult, 0 );
            if (iSendResult == SOCKET_ERROR) {
                cout << "envío fallido, error: " << WSAGetLastError() << endl;
                closesocket(ClientSocket);
                WSACleanup();
                return;
            }       
        }
        else if (iResult == 0)
            cout << "Cerrando conección\n";
        else  {
            cout << "mensaje recibido, error: " << WSAGetLastError() << endl;
            closesocket(ClientSocket);
            WSACleanup();
            return;
        }
    }while(iResult > 0);
    return 0;
 }

 int My_Server::shutdown_server(){
    // cerrando la conección
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        cout << "shutdown failed with error: " << WSAGetLastError() << endl;
        closesocket(ClientSocket);
        WSACleanup();
        return 1;
    }
 }

void My_Server::cleanup_server(){
    // limpiando el socjet
    closesocket(ClientSocket);
    WSACleanup();
}
int main(){
    return 0;
}