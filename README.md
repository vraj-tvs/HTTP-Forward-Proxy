# HTTP Forward Proxy

This project is an HTTP Forward Proxy implementation in C++ with a Next.js frontend, allowing users to forward requests through a proxy server to a designated backend server. This setup provides an added layer of anonymity and control, enabling users to route requests to multiple servers.

## Features

- **Forward Proxying**: Supports routing client requests to various servers through the proxy.
- **Flexible Backend Targeting**: Users can specify which server the proxy should forward their requests to.
- **Next.js Frontend Integration**: This proxy is compatible with a React-based frontend for a seamless user interface.

## Getting Started

### Prerequisites

- C++ compiler (e.g., GCC)
- Node.js and npm for the Next.js frontend

### Setup Instructions

1. **Clone the Repository**

   ```sh
   git clone https://github.com/vraj-tvs/HTTP-Forward-Proxy.git
   cd HTTP-Forward-Proxy
   ```
2. **Download the Nlohmann/JSON library**
    ```sh
    pacman -S mingw-w64-x86_64-nlohmann-json
    ```
3. **Compile the C++ files**

   ```sh
   g++ proxy.cpp -o proxy -lWs2_32
   g++ server.cpp -o server -lWs2_32
   ```

4. **Run the NextJS frontend**

   - Navigate to the frontend directory:
     ```sh
     cd frontend
     ```
   - Install dependencies:
     ```sh
     npm install
     ```
   - Start the development server:
     ```sh
     npm run dev
     ```
### Running the Servers
- Start the server:
    ```sh
    ./server.exe
    ```
- Run the proxy:
    ```sh
    ./proxy.exe
    ```
## Project Structure

```
HTTP-Forward-Proxy/
├── proxy.cpp         # Proxy server that forwards client requests
├── server.cpp        # Server that handles requests from the proxy
├── frontend/         # Next.js frontend for user interface
└── README.md         # Project documentation
```
    
## How It Works
1. The Next.js frontend sends HTTP requests to the proxy, specifying the backend server.
2. The proxy receives the request, forwards it to the specified server, and waits for a response.
3. The server processes the request and sends a response back to the proxy.
4. The proxy then relays the response back to the frontend.

## Future Enhancements
- Authentication and Access Control: Implement basic authentication for secure access.
- Caching: Add caching to reduce load on backend servers.
- Logging: Add request and response logging for monitoring.
