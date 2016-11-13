#include "stdafx.h"
#include "RequestFactory.h"


std::shared_ptr<Request> RequestFactory::createRequest(string_t url, string_t resource)
{
    auto request = std::make_shared<Request>(Request(url, resource));
    return request;
}