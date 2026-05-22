#include "content-types/json.h"
#include <iostream>
#include <map>
#include <string>
#include <sys/socket.h>

enum class HttpMethod  { Get, Post, Patch, Delete, Options, Put };
enum class ContentType { TextPlain, ApplicationJson };
enum class ResponseStatusText { Ok, NotFound, BadRequest };

class Request {
  int socket;

  public:
    std::string method;
    std::string referer;
    std::string host;
    std::map<std::string, std::string> cookies;
    std::string pathname;
};


struct ResponseHeaders {
  ContentType content_t;
};

class Response {
  int socket;
  ResponseStatusText status_text = ResponseStatusText::Ok;
  ResponseHeaders headers;

  private:
    std::string get_content_type_str() {
      switch(this->headers.content_t) {
        case ContentType::ApplicationJson: return "application/json";
        case ContentType::TextPlain: return "text/plain";
      }
      return "text/plain";
    }

    std::pair<int, std::string> get_status() {
      switch(this->status_text) {
        case ResponseStatusText::Ok: return {200, "OK"};
        case ResponseStatusText::NotFound: return {404, "NOT FOUND"};
        case ResponseStatusText::BadRequest: return {400, "BAD REQUEST"};

      };
      return {200, "OK"};
    }

  public:
    std::map<std::string, std::string> locals;

    Response(int s) : socket(s) {}

    void send(std::array<char, 1024> body) {
      std::pair<int, std::string> status = get_status();
    
      std::string d = 
      "HTTP/1.1 " + std::to_string(status.first) + " " + status.second
      + "\nContent-Type: " + get_content_type_str()
      + "\nContent-Length: " + std::to_string(body.size())
      + "\n\n" + body.data()
      ;

      ::send(this->socket, d.c_str(), d.length(), 0);
    };

    void json(JsonObject body) {
      

    }
};
