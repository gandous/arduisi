#pragma once
#include <Arduino.h>
#include <ESP8266WebServer.h>

typedef std::function<void(ESP8266WebServer &server)> callback_func_t;

inline ESP8266WebServer::THandlerFunction callback(ESP8266WebServer &server, callback_func_t fun)
{
    return ([&server, fun](){
        fun(server);
    });
}
