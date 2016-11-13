#include "stdafx.h"
#include "AnalyzeController.h"

#define DELIMITER ";"
AnalyzeController::AnalyzeController(const method& request_method, int request_count, const milliseconds& time_delta, string_t body, string_t filename)
{
    this->request_method = request_method;
    this->request_count = request_count;
    this->time_delta = time_delta;
    this->body = body;
    this->filename = filename;
    request_factory = std::make_shared<RequestFactory>(RequestFactory());
}

void AnalyzeController::addRequest(string_t url, string_t resource)
{
   analyzer_list.push_back(RequestAnalyzer(request_factory->createRequest(url, resource)));
}

void AnalyzeController::performTest(TestType type, const int request_id)
{
    switch (type){
    
    case TestType::Responsiveness:
        performResponsivnessTest(request_id);
        break;
    case TestType::Concurrency:
        performConcurrencyTest();
        break;
    default:
        break;
    }

}

void AnalyzeController::performResponsivnessTest(const int request_id)
{
    if (analyzer_list.size() > request_id)
    {
        auto test = analyzer_list[request_id].invokeRepeatedRequest(request_method, request_count, time_delta, body);
        try{
            test.wait();
        } catch (http_exception &e)
        {
            std::wcout << e.what() << std::endl;
            return;
        }
        dumpResultToFile(request_id, analyzer_list[request_id]);
    }
}

void AnalyzeController::performConcurrencyTest()
{
    std::vector<pplx::task<void>> tests;
    for (auto &a : analyzer_list)
    {
        tests.push_back(a.invokeRepeatedRequest(request_method, request_count, time_delta, body));
    }
    for (int i = 0; i < analyzer_list.size(); i++)
    {
        try{
            tests[i].wait();
        }
        catch (http_exception &e)
        {
            std::wcout << e.what() << std::endl;
            return;
        }
        dumpResultToFile(i, analyzer_list[i]);
    }
}

void AnalyzeController::dumpResultToFile(int id, RequestAnalyzer analyzer) const
{
    std::ofstream file;
    file.open(this->filename + L".csv", std::fstream::app);
    file << "REQUEST " << id << std::endl;
    file << DELIMITER << "Status" << DELIMITER<< "ReponseTime" << std::endl;
    for (auto r : analyzer.getResults())
    {
        file << DELIMITER << r.first << DELIMITER << r.second.count() << std::endl;
    }
    file << "MaxResponseTime" << DELIMITER << analyzer.getMaxResponseTime().count() << std::endl;
    file << "MinResponseTime" << DELIMITER << analyzer.getMinResponseTime().count() << std::endl;
    file << "AvarageResponseTime" << DELIMITER << analyzer.getAvarageResponseTime() << std::endl;
    file.close();
}
