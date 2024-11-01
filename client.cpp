#include <iostream>
#include <string>
#include <cstring>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/select.h>  // For select() on Unix systems
#include <errno.h>       // For errno on Unix systems
#endif

using namespace std;

#define PROXY_PORT 9090

int main() {
    int sock = 0;
    struct sockaddr_in proxy_addr;
    string user_message;

    #ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cout << "\033[31m[Client]: WSAStartup failed\033[0m" << endl;
        return -1;
    }
    #endif

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cout << "\033[31m[Client]: Socket creation error\033[0m" << endl;
        return -1;
    }
    cout << "\033[32m[Client]: Socket created successfully\033[0m" << endl;

    proxy_addr.sin_family = AF_INET;
    proxy_addr.sin_port = htons(PROXY_PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &proxy_addr.sin_addr) <= 0) {
        cout << "\033[31m[Client]: Invalid address\033[0m" << endl;
        return -1;
    }

    // Connect to the proxy server
    if (connect(sock, (struct sockaddr *)&proxy_addr, sizeof(proxy_addr)) < 0) {
        cout << "\033[31m[Client]: Connection to proxy failed\033[0m" << endl;
        return -1;
    }
    cout << "\033[32m[Client]: Connected to proxy!\033[0m" << endl;

    char recvbuf[1024];
    int recvbuflen = 1024;
    string sendbuf;
    int bytesReceived;

    while(true) {
        cout << "Client: ";
        getline(cin, sendbuf);
        send(sock, sendbuf.c_str(), sizeof(sendbuf), 0);

        if(sendbuf == "exit") {
            cout << "Client exiting the chat..." << endl;
            break;
        }

        bytesReceived = recv(sock, recvbuf, recvbuflen, 0);
        if(bytesReceived > 0) {
            recvbuf[bytesReceived] = '\0'; // Null-terminate received data

            if(strcmp(recvbuf, "exit") == 0) {
                cout << "\nServer has exited the chat." << endl;
                break;
            }
        }
    }

    cout << "\033[32m[Client]: Response from server: \033[0m" << recvbuf << endl;

    // Close the socket
    #ifdef _WIN32
    closesocket(sock);
    WSACleanup();
    #else
    close(sock);
    #endif

    return 0;
}
