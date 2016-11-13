#include "stdafx.h"
#include "RequestAnalyzer.h"
#include <thread>
#include <algorithm>
#include <numeric>
#include <chrono>

RequestAnalyzer::RequestAnalyzer(std::shared_ptr<Request> _request){
    request = _request;
}

pplx::task<void> RequestAnalyzer::invokeRepeatedRequest(const method mtd, const int count, const milliseconds time_delta, string_t body){
    return pplx::create_task([=]
    {
        for (int i=0; i < count; i++){
            std::this_thread::sleep_for(time_delta);
            auto task = request->invokeRequest(mtd, body);
            task.wait();
            auto result = std::pair<status_code, milliseconds>(
                task.get().status_code(),
                request->getLastResponseTime());
            results.push_back(result);
        }
    });
}

std::vector<std::pair<status_code, milliseconds>> RequestAnalyzer::getResults() const
{
    return results;
}

std::wstring RequestAnalyzer::getRequestURI()
{
    return request->getUrl() + request->getResource();
}

void RequestAnalyzer::setRequest(std::shared_ptr<Request> r)
{
	request = r;
	results.clear();
}

milliseconds RequestAnalyzer::getLastResponseTime(){
    return results.back().second;
}

milliseconds RequestAnalyzer::getMaxResponseTime(){
    return std::max_element(
            results.begin(),
            results.end(), 
            [](const std::pair<status_code, milliseconds> &a, const std::pair<status_code, milliseconds> &b){
                return (a.second < b.second);
            })->second;
}

milliseconds RequestAnalyzer::getMinResponseTime(){
    return std::min_element(
            results.begin(),
            results.end(),
            [](const std::pair<status_code, milliseconds> &a, const std::pair<status_code, milliseconds> &b){
            return (a.second < b.second);
            })->second;
}

long RequestAnalyzer::getAvarageResponseTime(){
    long sum = 0;
    for (auto r : results)
        sum += r.second.count();
    return sum / results.size();
}