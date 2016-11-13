#pragma once
#include "restformance.h"
#include <cpprest/http_client.h>
#include <chrono>

using namespace utility;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace std::chrono;

class RESTFORMANCEDLL_API Request{

    string_t url;
    string_t resource;
    high_resolution_clock::time_point start;
    high_resolution_clock::time_point end;
    pplx::task<http_response> request_task;

public:
    Request(string_t _url, string_t _resource) : url(_url), resource(_resource){}
    
    // Getters and Setters
    void setUrl(string_t _url);

    string_t getUrl() const;

    void setResource(string_t _resource){
        resource =_resource;
    }

    string_t getResource() const;
    pplx::task<http_response> invokeRequest(method mtd, string_t body);
    milliseconds getLastResponseTime() const;
};