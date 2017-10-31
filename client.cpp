
#include "client.h"

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
using namespace std;


My_Client::My_Client(const int _DEFAULT_BUFLEN){
    DEFAULT_BUFLEN = _DEFAULT_BUFLEN;

    ZeroMemory( &hints, sizeof(hints) );//Zero Memory es una funcion que pone todos los valores en 0s    
    hints.ai_family = AF_UNSPEC;//la direccion no esta especificada
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
}

int My_Client::init_Components(){
    // Iniciar los componentes de Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        cout << "error al iniciar el programa: " << iResult << endl;
        return 1;
    }
}
int My_Client::connect_port(){
    // añadir el la direccion del servidor y el puerto de conección
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        cout << "la funcion getaddrinfo fallo, error: " << iResult << endl;
        WSACleanup();
        return 1;
    }
}

int My_Client::create_socket(){

    // conectar a la primera dierccioon enviada
    // la llamada a getaddrinfo
    ptr=result;

    // creo el socket para la conecion
    ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,ptr->ai_protocol);

    if (ConnectSocket == INVALID_SOCKET) {
        cout << "Error at socket(): " << WSAGetLastError() << endl;
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }
}

int My_Client::connect_server(){
    // Connect to server.
    iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        closesocket(ConnectSocket);
        ConnectSocket = INVALID_SOCKET;
        return 1;
    }

    // Should really try the next address returned by getaddrinfo
    // if the connect call failed
    // But for this simple example we just free the resources
    // returned by getaddrinfo and print an error message

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        cout << "no se puede conectar al server!\n";
        WSACleanup();
        return 1;
    }
}

My_Client::send_and_receive_message(){
    char message[DEFAULT_BUFLEN];
    do{
        cout << " > ";
        cin.getline(message, DEFAULT_BUFLEN);
        // Send an initial buffer
        iResult = send(ConnectSocket, message, (int) strlen(message), 0);
        if (iResult == SOCKET_ERROR) {
            cout << "envio fallo: " << WSAGetLastError() << endl;
            closesocket(ConnectSocket);
            WSACleanup();
            return 1;
    }
    else{
        ZeroMemory(message,DEFAULT_BUFLEN);
        iResult = recv(ConnectSocket, message, DEFAULT_BUFLEN, 0);
        if (iResult > 0){
            cout << "Server: " << message << endl;
        }
        else if (iResult == 0)
            cout << "coneccion cerrada\n";
        else
            cout << "recibo fallo: " << WSAGetLastError() << endl;
    }   
    }while(iResult > 0);
}
   
int My_Client::shutdown_client(){
    // apagando al cliente
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        cout << "shutdown failed: << "<< WSAGetLastError() << endl;
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }
} 

int main(int argc, char **argv) {
 
  return 0;
}