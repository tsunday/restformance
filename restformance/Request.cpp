#include "stdafx.h"
#include "Request.h"
#include <chrono>


void Request::setUrl(string_t _url)
{
    url = _url;
}

string_t Request::getUrl() const
{
    return url;
}

string_t Request::getResource() const
{
    return resource;
}

pplx::task<http_response> Request::invokeRequest(method mtd, string_t body){
    request_task = pplx::create_task([=]
    {
        http_client client(url);
        uri_builder builder(resource);
        start = high_resolution_clock::now();
        if (mtd != methods::GET)
            return client.request(mtd, builder.to_string(), body, L"application/json");
        return client.request(mtd, builder.to_string());

    }).then([=](http_response r)
    {
        end = high_resolution_clock::now();
        printf("Received response status code:%u\n", r.status_code());
        return r;

    });
    return request_task;
}

milliseconds Request::getLastResponseTime() const
{
    return duration_cast<milliseconds>(end - start);
}

