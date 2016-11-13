#pragma once
#include "RequestAnalyzer.h"
#include "RequestFactory.h"

enum class RESTFORMANCEDLL_API TestType{
    Responsiveness,
    Concurrency
};

class RESTFORMANCEDLL_API AnalyzeController{

    std::shared_ptr<RequestFactory> request_factory;
    std::vector<RequestAnalyzer> analyzer_list;
    method request_method;
    int request_count;
    milliseconds time_delta;
    string_t body;
    string_t filename;

public:

    method getMethod() const
    {
        return request_method;
    }

    void setMethod(const method& request_method)
    {
        this->request_method = request_method;
    }

    int getCount() const
    {
        return request_count;
    }

    void setCount(int request_count)
    {
        this->request_count = request_count;
    }

    milliseconds getTimeDelta() const
    {
        return time_delta;
    }

    void setTimeDelta(const milliseconds& time_delta)
    {
        this->time_delta = time_delta;
    }


    string_t get_body() const
    {
        return body;
    }

    void set_body(const string_t& body)
    {
        this->body = body;
    }

    AnalyzeController(const method& request_method, int request_count, const milliseconds& time_delta, const string_t body, const string_t filename);
    void addRequest(string_t url, string_t resource);
    void performTest(TestType type, const int request_id=0);
    void performResponsivnessTest(const int request_id);
    void performConcurrencyTest();
    void dumpResultToFile(int id, RequestAnalyzer analyzer) const;
};