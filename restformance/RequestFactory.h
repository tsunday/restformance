#pragma once
#include "Request.h"

class RESTFORMANCEDLL_API RequestFactory{

public:
    static std::shared_ptr<Request> createRequest(string_t url, string_t resource);
};