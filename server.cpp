#include <iostream>
#include <cstring>  // For memset
#include <thread>  // For multi-threading

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

using namespace std;

int main() {
    int server_sock = 0, proxy_sock = 0;
    struct sockaddr_in server_addr, proxy_addr;
    char buffer[1024] = {0};
    int addrlen = sizeof(proxy_addr);

    #ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    #endif

    // Create server socket
    if ((server_sock = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        cout << "\033[31m[Server]: Socket creation failed\033[0m" << endl;
        return -1;
    }
    cout << "\033[32m[Server]: Socket created successfully\033[0m" << endl;

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT);

    // Bind the socket to the server address
    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        cout << "\033[31m[Server]: Bind failed\033[0m" << endl;
        return -1;
    }
    cout << "\033[32m[Server]: Bind successful\033[0m" << endl;

    // Listen for incoming connections from the proxy
    if (listen(server_sock, 5) < 0) {
        cout << "\033[31m[Server]: Listen failed\033[0m" << endl;
        return -1;
    }
    cout << "\033[33m[Server]: Waiting for proxy connection...\033[0m" << endl;

    // Accept connection from proxy
    if ((proxy_sock = accept(server_sock, (struct sockaddr *)&proxy_addr, (socklen_t*)&addrlen)) < 0) {
        cout << "\033[31m[Server]: Proxy accept failed\033[0m" << endl;
        return -1;
    }
    cout << "\033[32m[Server]: Proxy connected!\033[0m" << endl;

    // Server will stay open as long as the proxy is connected
    while (true) {
        // Clear the buffer
        memset(buffer, 0, sizeof(buffer));

        // Receive message from the proxy
        int valread = recv(proxy_sock, buffer, sizeof(buffer), 0);
        if (valread <= 0) {
            if (valread == 0) {
                cout << "\033[31m[Server]: Proxy disconnected. Shutting down server...\033[0m" << endl;
            } else {
                cout << "\033[31m[Server]: Error receiving message from proxy\033[0m" << endl;
            }
            break;  // Exit loop and close the server
        }

        // Print the received message from proxy
        cout << "\033[32m[Server]: Message received: \033[0m" << buffer << endl;

        // Respond to the proxy
        const char* response = "Message received by server";
        send(proxy_sock, response, strlen(response), 0);
    }

    // Close the proxy and server sockets
    #ifdef _WIN32
    closesocket(proxy_sock);
    closesocket(server_sock);
    WSACleanup();
    #else
    close(proxy_sock);
    close(server_sock);
    #endif

    cout << "\033[33m[Server]: Server shut down successfully.\033[0m" << endl;

    return 0;
}
