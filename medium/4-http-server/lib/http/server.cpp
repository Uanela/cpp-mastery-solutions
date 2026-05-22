#include <iostream>
#include "core.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

struct InitReturn {
  int server_fd;
  sockaddr_in address;
};


std::vector<std::string> split_str(std::string s, char del = ' ') {
    std::vector<std::string> lines;
    std::stringstream ss(s);
    std::string line;

    while (getline(ss, line, del)) {
      lines.push_back(line);
    }

  return lines;
}

std::string trim_str(const std::string& s) {
    size_t start = 0;
    while (start < s.size() && std::isspace(s[start])) start++;

    size_t end = s.size();
    while (end > start && std::isspace(s[end - 1])) end--;

    return s.substr(start, end - start);
}

using Handler = std::function<void(Request, Response)>;

class Server {
  private:
    InitReturn init(int p) {
      // Create a socket (IPv4, TCP)
      int server_fd = socket(AF_INET, SOCK_STREAM, 0);

      // Define server address
      sockaddr_in address;
      address.sin_family = AF_INET;
      address.sin_addr.s_addr = INADDR_ANY; // Accept from any IP
      address.sin_port = htons(p);

      return { server_fd, address };
    }

    Handler handler;

    void manipulate(int s, sockaddr_in a) {
      while (true) {
          // Accept connection
          int addrlen = sizeof(a);
          int new_socket = accept(s, (struct sockaddr*)&a, (socklen_t*)&addrlen);
          
          const int buf_size = 1024;
          char buf[buf_size] = {0};
          read(new_socket, buf, buf_size);

          Request req = this->get_req(buf);
          Response res = Response();


          

          this->handler(req, res);
      };
    }

  // HttpMethod get_method(std::string m) {
  //   if (m == "GET") return 
  // }

  Request get_req(char buf[]) {
    std::vector<std::string> buf_lines = split_str(buf, '\n');
    Request req;

    std::vector<std::string> core_detail = split_str(buf_lines[0]);
    req.method = core_detail[0];
    req.pathname = core_detail[1];

    for (std::string l : buf_lines ) {
      std::vector<std::string> content = split_str(l, ':');
      std::string k = content[0];
      std::string v = trim_str(content[1]);

      if (k == "Host") req.host = v;
      if (k == "Referer") req.referer = v;
      // if (k == "Cookie") req.cookies = v;
    
    };

    return req;
  }

  public:
    Server (Handler h) : handler(h) {
      
    }

    Server& listen(int p, std::function<void()> c) {
      InitReturn init_inf = init(p);
      int s_fd = init_inf.server_fd;
      sockaddr_in addr = init_inf.address;

      // Bind the socket to the port
      int r = bind(s_fd, (struct sockaddr*)&addr, sizeof(addr));
      if (r == -1) throw "port " + (char{4})p + " is already in use.";

      // Listen
      ::listen(s_fd, 3);

      this->manipulate(s_fd, addr);
      
      c();

      return *this;
    }

};
