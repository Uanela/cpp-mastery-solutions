#include <iostream>
#include "core.h"
#include <sys/socket.h>
#include <netinet/in.h>

class Server {

  public:
    Server (Request req, Response res) {

      int server_fd = socket(AF_INET, SOCK_STREAM, 0);

      // Define server address
      sockaddr_in address;
      address.sin_family = AF_INET;
      address.sin_addr.s_addr = INADDR_ANY; // Accept from any IP

    }

    Server& listen(int port) {

      return *this;
    }

};
