#pragma once
#include "Request.h"
#include <memory>
#include <vector>

class RESTFORMANCEDLL_API RequestAnalyzer {

    std::shared_ptr<Request> request;
    std::vector<std::pair<status_code, milliseconds>> results;

public:
    RequestAnalyzer() {}
    RequestAnalyzer(std::shared_ptr<Request> _request);
    pplx::task<void> invokeRepeatedRequest(const method mtd, const int count, const milliseconds time_delta, string_t body);
    std::vector<std::pair<status_code, milliseconds>> getResults() const;
    std::wstring getRequestURI();
    void setRequest(std::shared_ptr<Request> r);
    milliseconds getLastResponseTime();
    milliseconds getMaxResponseTime();
    milliseconds getMinResponseTime();
    long getAvarageResponseTime();
};
