#pragma once

#include <iostream>
#include <map>

enum class HttpMethod  { Get, Post, Patch, Delete, Options, Put };

class Request {
  int socket;

  public:
    std::string method;
    std::string referer;
    std::string host;
    std::map<std::string, std::string> cookies;
    std::string pathname;
};

class Response {
  public:
    std::map<std::string, std::string> locals;

    void send(char data[1024]) {};

    void json() {}
};
