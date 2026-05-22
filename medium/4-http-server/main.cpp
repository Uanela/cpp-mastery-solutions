#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main() {
  // 1. Create a socket (IPv4, TCP)
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // 2. Define server address
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // Accept from any IP
    address.sin_port = htons(8080);       // Listen on Port 8080

    // 3. Bind socket to the port
    bind(server_fd, (struct sockaddr*)&address, sizeof(address));

    // 4. Start listening
    listen(server_fd, 3);
    std::cout << "Server started on port 8080..." << std::endl;

    while (true) {
        // 5. Accept a connection
        int addrlen = sizeof(address);
        int new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);

        // 6. Read Request
        char buffer[1024] = {0};
        read(new_socket, buffer, 1024);
        std::cout << "Request received: " << buffer << std::endl;

        // 7. Send valid HTTP Response
        std::string hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello World!";
        send(new_socket, hello.c_str(), hello.length(), 0);

        close(new_socket);
    }
    return 0;

  return 0;
}
