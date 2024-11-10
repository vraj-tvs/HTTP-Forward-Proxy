// util.h
#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
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
#include <sys/select.h>  // For select() on Unix systems
#include <errno.h>       // For errno on Unix systems
#endif

#endif // UTILS_H