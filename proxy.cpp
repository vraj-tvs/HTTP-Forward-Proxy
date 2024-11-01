#include <iostream>
#include <vector>
#include <atomic>
#include <cstring>  // For memset
#include <thread> // for multi-threading

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

#define SERVER_PORT 8080
#define PROXY_PORT 9090

using namespace std;

void handleClient(SOCKET client_sock, SOCKET server_sock){
    int bufferSize = 1024;
    char clientBuffer[bufferSize] = {0};
    char senderBuffer[bufferSize] = {0};

    while (true) {
        int bytesReceived = recv(client_sock, clientBuffer, 1024, 0);
        if (bytesReceived <= 0) {
            cout << "\033[31m[Proxy]: Client disconnected (FD " << client_sock << ")\033[0m" << endl;
            break;
        }
        else{
            clientBuffer[bytesReceived] = '\0';
            cout << "\033[33m[Proxy]: Message from client (FD " << client_sock << "): " << clientBuffer << "\033[0m" << endl;

            // Forward message to the server
            send(server_sock, clientBuffer, bytesReceived, 0);

            // Receive response from server
            bytesReceived = recv(server_sock, senderBuffer, bufferSize, 0);
            senderBuffer[bytesReceived] = '\0';

            // Send the response back to the client
            send(client_sock, senderBuffer, bytesReceived, 0);
            if(strcmp(clientBuffer, "exit") == 0){
                cout << "\033[32m[Proxy]: Message of Server forwarded to client (FD " << client_sock << ")\033[0m" << endl;
            }
        }
    }

    #ifdef _WIN32
    closesocket(client_sock);
    #else
    close(client_sock);
    #endif
}


int main() {
    SOCKET server_sock, proxy_sock, client_sock;
    struct sockaddr_in server_addr, proxy_addr, client_addr;
    int addrlen = sizeof(client_addr);

    #ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    #endif

    // Create proxy socket
    if ((proxy_sock = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        cout << "\033[31m[Proxy]: Socket creation failed\033[0m" << endl;
        return -1;
    }
    cout << "\033[32m[Proxy]: Socket created successfully\033[0m" << endl;

    // Setup proxy address
    proxy_addr.sin_family = AF_INET;
    proxy_addr.sin_addr.s_addr = INADDR_ANY;
    proxy_addr.sin_port = htons(PROXY_PORT);

    // Bind proxy socket to address
    if (bind(proxy_sock, (struct sockaddr *)&proxy_addr, sizeof(proxy_addr)) < 0) {
        cout << "\033[31m[Proxy]: Bind failed\033[0m" << endl;
        return -1;
    }
    cout << "\033[32m[Proxy]: Bind successful\033[0m" << endl;

    // Connect to the server
    if ((server_sock = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        cout << "\033[31m[Proxy]: Failed to create server socket\033[0m" << endl;
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    // Convert localhost to binary address
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        cout << "\033[31m[Proxy]: Invalid server address\033[0m" << endl;
        return -1;
    }

    if (connect(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        cout << "\033[31m[Proxy]: Connection to server failed\033[0m" << endl;
        return -1;
    }
    cout << "\033[32m[Proxy]: Connected to server\033[0m" << endl;
    
    // Listen for client connections
    if (listen(proxy_sock, 5) < 0) {
        cout << "\033[31m[Proxy]: Listen failed\033[0m" << endl;
        return -1;
    }
    cout << "\033[33m[Proxy]: Waiting for client connections...\033[0m" << endl;

    // Multi-client handling logic (multi-threaded)
    while(true){
        client_sock = accept(proxy_sock, (struct sockaddr *)&client_addr, (socklen_t *)&addrlen);
        if (client_sock == INVALID_SOCKET) {
            cout << "\033[31m[Proxy]: Client accept failed\033[0m" << endl;
            continue;
        }
        cout << "\033[32m[Proxy]: Client connected (FD " << client_sock << ")\033[0m" << endl;

        thread client_thread(handleClient, client_sock, server_sock);
        client_thread.detach();
    }

    #ifdef _WIN32
    closesocket(proxy_sock);
    closesocket(server_sock);
    WSACleanup();
    #else
    close(proxy_sock);
    close(server_sock);
    #endif

    return 0;
}
