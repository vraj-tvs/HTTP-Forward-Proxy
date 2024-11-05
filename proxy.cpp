#include <iostream>
#include <map>
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

#define PROXY_PORT 9090

using namespace std;

// Map to store client socket and server assignment
map<SOCKET, int> clientIDMap;
map<int, int> clientToServerMap;
map<int, SOCKET> serverSockets;
int uniqueClientID = 1; // Simple counter to generate unique IDs

// Function to handle each client connection
void handleClient(SOCKET client_sock, SOCKET server_sock, int clientID, int serverID){
    int bufferSize = 1024;
    char clientBuffer[bufferSize] = {0};
    char serverBuffer[bufferSize] = {0};

    while (true) {
        int bytesReceived = recv(client_sock, clientBuffer, 1024, 0);
        if (bytesReceived <= 0) {
            cout << "\033[31m[Proxy]: Client disconnected (ClientID " << clientID << ")\033[0m" << endl;
            break;
        }

        clientBuffer[bytesReceived] = '\0';
        cout << "\033[33m[Proxy]: Message from client (ClientID " << clientID << "): " << clientBuffer << "\033[0m" << endl;

        // Forward message to the server
        string messageWithID = to_string(clientID) + ":" + clientBuffer;
        send(server_sock, messageWithID.c_str(), sizeof(messageWithID), 0);

        // Receive response from server
        bytesReceived = recv(server_sock, serverBuffer, bufferSize, 0);
        serverBuffer[bytesReceived] = '\0';

        // Parse client ID from server response
        string message(serverBuffer);
        int delimiter = message.find(":");
        int clientID = stoi(message.substr(0, delimiter));
        string actualMessage = message.substr(delimiter + 1);

        // Send the response back to the client
        send(client_sock, actualMessage.c_str(), sizeof(actualMessage), 0);
        if(strcmp(clientBuffer, "exit") == 0){
            cout << "\033[32m[Proxy]: Message of Server " << serverID << " forwarded to client " << clientID << "\033[0m" << endl;
            break;
        }
        
    }

    #ifdef _WIN32
    closesocket(client_sock);
    #else
    close(client_sock);
    #endif
}

// Function to accept and assign clients to servers
void acceptClients(SOCKET proxy_sock, int numServers){
    SOCKET client_sock;
    struct sockaddr_in client_addr;
    int addrlen = sizeof(client_addr);
    
    // Multi-client handling logic (multi-threaded)
    while(true){
        client_sock = accept(proxy_sock, (struct sockaddr *)&client_addr, (socklen_t *)&addrlen);
        if (client_sock == INVALID_SOCKET) {
            cout << "\033[31m[Proxy]: Client accept failed\033[0m" << endl;
            continue;
        }

        // Assign a unique ID and inform the client
        int clientID = uniqueClientID++;
        clientIDMap[client_sock] = clientID;
        send(client_sock, to_string(clientID).c_str(), sizeof(to_string(clientID)), 0); // Send ID to client
        
        // Assign the requested server to the client
        char serverIdBuffer[1024] = {0};
        recv(client_sock, serverIdBuffer, sizeof(serverIdBuffer), 0);
        int serverID = stoi(serverIdBuffer);
        SOCKET server_sock = serverSockets[serverID];
        clientToServerMap[clientID] = serverID;

        // TODO: assignment of client to some other server if the server with no. ServerID isn't connected to proxy

        cout << "\033[32m[Proxy]: Client " << clientID << " connected and assigned to Server " << serverID << "\033[0m" << endl;

        thread client_thread(handleClient, client_sock, server_sock, clientID, serverID);
        client_thread.detach();
    }
}

int main() {
    int numServers = 3; // Define the number of servers
    int serverPorts[] = {8081, 8082, 8083}; // List of server ports

    SOCKET proxy_sock;
    struct sockaddr_in proxy_addr;

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

    // Initialize connections to all servers
    for (int i = 0; i < numServers; i++) {
        SOCKET server_sock;
        struct sockaddr_in server_addr;

        // Connect to the server
        if ((server_sock = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
            cout << "\033[31m[Proxy]: Failed to create server socket\033[0m" << endl;
            return -1;
        }

        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(serverPorts[i]);

        // Convert localhost to binary address
        if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
            cout << "\033[31m[Proxy]: Invalid server address\033[0m" << endl;
            // return -1;
            continue;
        }

        if (connect(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            cout << "\033[31m[Proxy]: Connection to Server " << i+1 << " failed\033[0m" << endl;
            // return -1;
            continue;
        }
        
        serverSockets[i + 1] = server_sock;
        cout << "\033[32m[Proxy]: Connected to Server " << i+1 << "\033[0m" << endl;
    }

    // Listen for client connections
    if (listen(proxy_sock, 10) < 0) {
        cout << "\033[31m[Proxy]: Listen failed\033[0m" << endl;
        return -1;
    }
    cout << "\033[33m[Proxy]: Waiting for client connections...\033[0m" << endl;

    thread acceptThread(acceptClients, proxy_sock, numServers);
    acceptThread.join();

    #ifdef _WIN32
    closesocket(proxy_sock);
    WSACleanup();
    #else
    close(proxy_sock);
    #endif

    return 0;
}
