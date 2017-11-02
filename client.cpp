#define WIN32_LEAN_AND_MEAN

#define _WIN32_WINNT 0x501

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <iostream>
#include <string>

using namespace std;
// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"
class myClient
{
    string ipAddress;
    string port;
    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;
    string sendbuf;
    int iResult;
    int recvbuflen;
    
public:
    myClient(const string & _ipAddress, const string & _port, int _recvbuflen) : ipAddress(_ipAddress) , 
    port(_port) , recvbuflen(_recvbuflen){}
    int runclient();
    ~myClient(){}
};
int myClient::runclient(){
// Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        cout << "WSAStartup failed with error: " << iResult;
        return 1;
    }
    cout << "connected with " << ipAddress << endl;
    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(ipAddress.c_str(), DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        cout << "getaddrinfo failed with error: " << iResult << endl;
        WSACleanup();
        return 1;
    }

    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            cout << "socket failed with error: " << WSAGetLastError() << endl;
            WSACleanup();
            return 1;
        }

        // Connect to server.
        iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        //break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        cout << "Unable to connect to server!\n";
        WSACleanup();
        return 1;
    }
    char recvbuf[recvbuflen];
do{
    cout << "> ";
    getline(cin, sendbuf);
    // Send an initial buffer
    iResult = send( ConnectSocket, sendbuf.c_str(), sendbuf.size(), 0 );
    if (iResult == SOCKET_ERROR) {
        cout << "send failed with error: " << WSAGetLastError() << endl;
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }
    ZeroMemory(recvbuf,recvbuflen);
    iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if ( iResult > 0 )
            cout << "Server> " << string(recvbuf, 0 , iResult+1) << endl;
        
   
}while(sendbuf != "exit");
 
    cout << "Bytes Sent: " << iResult << endl;

    // shutdown the connection since no more data will be sent
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        cout << "shutdown failed with error: " << WSAGetLastError() << endl;
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    // Receive until the peer closes the connection
    do {
        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if ( iResult > 0 )
            cout << "Bytes received: " << iResult << endl;
        else if ( iResult == 0 )
            cout << "Connection closed\n";
        else
            cout << "recv failed with error: " << WSAGetLastError() << endl;

    } while( iResult > 0 );

    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();


}
int main()
{
    myClient hola("127.0.0.1", "27015" , 512);
    hola.runclient();
    
    return 0;
}