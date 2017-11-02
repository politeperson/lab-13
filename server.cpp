#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#define _WIN32_WINNT 0x501

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <iostream>
#include <string>

using namespace std;
// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

class myServer
{
private:
    WSADATA wsaData;
    int iResult;

    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;

    struct addrinfo *result = NULL;
    struct addrinfo hints;
    
    #define DEFAULT_PORTi "27015"

    string DEFAULT_PORT;
    int iSendResult;
    int recvbuflen;

public:
    myServer(int _buflen, const string & PORT){
        recvbuflen = _buflen;
        DEFAULT_PORT = PORT;
    }
    int init(){
        // Initialize Winsock
        iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
        if (iResult != 0) {
            cout << "WSAStartup failed with error: " << iResult << endl;
            return 1;
        }
        else cout << "ready to use winsock" << endl;
        cout << "> ";
        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        hints.ai_flags = AI_PASSIVE;
    }
    int send_message(char recvbuf[] , int recvbuflen){
        cin.getline(recvbuf,recvbuflen,'\n');
        iSendResult = send( ClientSocket, recvbuf, iResult, 0 );
        if (iSendResult == SOCKET_ERROR) {
            cout << "send failed with error: " << WSAGetLastError() << endl;
            closesocket(ClientSocket);
            WSACleanup();
            return 1;
        }
        ZeroMemory(recvbuf,recvbuflen);
    }
    int runserver(){
        init();
        // Resolve the server address and port
        iResult = getaddrinfo(NULL, DEFAULT_PORT.c_str(), &hints, &result);
        if ( iResult != 0 ) {
            cout << "getaddrinfo failed with error: " << iResult << endl;
            WSACleanup();
            return 1;
        }
        // Create a SOCKET for connecting to server
        ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
        if (ListenSocket == INVALID_SOCKET) {
            cout << "socket failed with error: " << WSAGetLastError() << endl;
            freeaddrinfo(result);
            WSACleanup();
            return 1;
        }
            // Setup the TCP listening socket
        iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            cout << "bind failed with error: " << WSAGetLastError() << endl;
            freeaddrinfo(result);
            closesocket(ListenSocket);
            WSACleanup();
            return 1;
        }

        freeaddrinfo(result);

        iResult = listen(ListenSocket, SOMAXCONN);
        if (iResult == SOCKET_ERROR) {
            cout << "listen failed with error: " << WSAGetLastError() << endl;
            closesocket(ListenSocket);
            WSACleanup();
            return 1;
        }
       // Accept a client socket
        ClientSocket = accept(ListenSocket, NULL, NULL);
        if (ClientSocket == INVALID_SOCKET) {
            cout << "accept failed with error: " << WSAGetLastError() << endl;
            closesocket(ListenSocket);
            WSACleanup();
            return 1;
        }
        cout << "client connected...\n";
        // No longer need server socket
        closesocket(ListenSocket);
    
        char recvbuf[recvbuflen];
            // Receive until the peer shuts down the connection
        do {
            iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
            if (iResult > 0) {
                cout << "Client> " << string(recvbuf, 0 , iResult) << endl;
                cout << "> ";
                ZeroMemory(recvbuf,iResult);
                send_message(recvbuf, recvbuflen);
            }
            else if (iResult == 0)
                cout << "Connection closing...\n";
            else  {
                cout << "recv failed with error: " << WSAGetLastError() << endl;
                closesocket(ClientSocket);
                WSACleanup();
                return 1;
            }

        } while (iResult > 0);

        // shutdown the connection since we're done
        iResult = shutdown(ClientSocket, SD_SEND);
        if (iResult == SOCKET_ERROR) {
            cout << "shutdown failed with error: " << WSAGetLastError() << endl;
            closesocket(ClientSocket);
            WSACleanup();
            return 1;
        }
        cout << "bye Admin\n";
        // cleanup
        closesocket(ClientSocket);
        WSACleanup();
    }
    ~myServer(){}
};



int main() 
{
    cout << "Welcome Admin\n";
    cout << "your IP Address is: 127.0.0.1\nyour port connection is: 27015\nmaximun characters 512\n";
    myServer hola(512, "27015");
    hola.runserver();
    hola.~myServer();
    return 0;
}