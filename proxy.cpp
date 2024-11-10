#include <iostream>
#include <vector>
#include <atomic>
#include <cstring>  // For memset
#include <nlohmann/json.hpp>
using json = nlohmann::json;
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

// Global variable to indicate if the proxy should stop
atomic<bool> should_stop(false);

struct Server {
    string address;
    int port;
    string identifier;
    SOCKET socket;
};

// Global list of available servers
vector<Server> servers = {
    {"127.0.0.1", 8081,"server1"},  // Server 1
     {"127.0.0.1", 8082,"server2"},  // Server 2
    {"127.0.0.1", 8083,"server3"}   // Server 3
};

int findServerSocket(const string& selectedServer) {
    for (auto& server : servers) {
        if (server.identifier == selectedServer) {
            return server.socket;
        }
    }
    return -1; // Server not found
}

int main() {
    SOCKET proxy_sock, client_sock;
    struct sockaddr_in proxy_addr, client_addr;
    int addrlen = sizeof(client_addr);
    const int bufferSize = 1024;

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
    if (bind(proxy_sock, (struct sockaddr*)&proxy_addr, sizeof(proxy_addr)) < 0) {
        cout << "\033[31m[Proxy]: Bind failed\033[0m" << endl;
        return -1;
    }
    cout << "\033[32m[Proxy]: Bind successful\033[0m" << endl;

    // Listen for client connections
    if (listen(proxy_sock, 5) < 0) {
        cout << "\033[31m[Proxy]: Listen failed\033[0m" << endl;
        return -1;
    }
    cout << "\033[33m[Proxy]: Waiting for client connections...\033[0m" << endl;

    // Create server sockets for each server
    for (auto& server : servers) {
        server.socket = socket(AF_INET, SOCK_STREAM, 0);
        struct sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(server.port);

        if (inet_pton(AF_INET, server.address.c_str(), &server_addr.sin_addr) <= 0) {
            cout << "\033[31m[Proxy]: Invalid server address: " << server.address << "\033[0m" << endl;
            return -1;
        }

        if (connect(server.socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            cout << "\033[31m[Proxy]: Connection to server " << server.address << " failed\033[0m" << endl;
            return -1;
        }

        cout << "\033[32m[Proxy]: Connected to server " << server.address << "\033[0m" << endl;
    }

    // Multi-client handling logic
    fd_set masterSet, readSet;
    vector<SOCKET> clientSockets;
    int max_sd = proxy_sock;

    FD_ZERO(&masterSet);
    FD_SET(proxy_sock, &masterSet);

    while (!should_stop) {
        readSet = masterSet;

        int activity = select(max_sd + 1, &readSet, NULL, NULL, NULL);
        if (activity < 0 && errno != EINTR) {
            cout << "\033[31m[Proxy]: Select failed\033[0m" << endl;
            break;
        }

        if (FD_ISSET(proxy_sock, &readSet)) {
            client_sock = accept(proxy_sock, (struct sockaddr*)&client_addr, (socklen_t*)&addrlen);
            if (client_sock == INVALID_SOCKET) {
                cout << "\033[31m[Proxy]: Client accept failed\033[0m" << endl;
                continue;
            }
            cout << "\033[32m[Proxy]: Client connected (FD " << client_sock << ")\033[0m" << endl;

            // Add new client to the master set
            FD_SET(client_sock, &masterSet);
            clientSockets.push_back(client_sock);
            if (client_sock > max_sd) {
                max_sd = client_sock;
            }
        }

        for (auto it = clientSockets.begin(); it != clientSockets.end(); ) {
            SOCKET clientSocket = *it;
            if (FD_ISSET(clientSocket, &readSet)) {
                char clientBuffer[bufferSize] = { 0 };
                char senderBuffer[bufferSize] = { 0 };
                int bytesReceived = recv(clientSocket, clientBuffer, bufferSize, 0);

                if (bytesReceived <= 0) {
                    // Client disconnect or error
                    closesocket(clientSocket);
                    FD_CLR(clientSocket, &masterSet);
                    it = clientSockets.erase(it);
                    cout << "\033[31m[Proxy]: Client disconnected (FD " << clientSocket << ")\033[0m" << endl;
                }
                else {
                    clientBuffer[bytesReceived] = '\0';
                    cout << "\033[33m[Proxy]: Message from client (FD " << clientSocket << "): " << clientBuffer << "\033[0m" << endl;

                    string selectedServer;
                    string messageRecv;

                    try {
                        json parsedJson = json::parse(clientBuffer);
                        selectedServer = parsedJson.at("selectedServer").get<string>();
                        messageRecv = parsedJson.at("message").get<string>();
                    }
                    catch (const std::exception& e) {
                        std::cerr << "Error parsing JSON: " << e.what() << '\n';
                    }

                    // string selectedServer(clientBuffer);  // For simplicity, assume the client sends the server address as the message
                    int server_socket = findServerSocket(selectedServer);


                    if (server_socket != -1) {
                        // Forward message to the selected server
                        send(server_socket, messageRecv.c_str(), messageRecv.size(), 0);

                        // Receive response from the server
                        bytesReceived = recv(server_socket, senderBuffer, bufferSize, 0);
                        senderBuffer[bytesReceived] = '\0';

                        // Send the response back to the client
                        send(clientSocket, senderBuffer, bytesReceived, 0);
                        cout << "\033[32m[Proxy]: Message forwarded to server and response sent to client (FD " << clientSocket << ")\033[0m" << endl;
                    }
                    else {
                        cout << "\033[31m[Proxy]: Invalid server selected by client\033[0m" << endl;
                        const char* errorMsg = "Invalid server selected";
                        send(clientSocket, errorMsg, strlen(errorMsg), 0);
                    }
                    ++it;
                }
            }
            else {
                ++it;
            }
        }
    }

    // Cleanup
    for (auto sock : clientSockets) {
        closesocket(sock);
    }

    for (auto& server : servers) {
        closesocket(server.socket);
    }

#ifdef _WIN32
    WSACleanup();
#endif

    return 0;
}
