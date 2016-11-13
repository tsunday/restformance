#pragma once
#include "stdafx.h"
#include "Balancer.h"
#include <chrono>
#include <algorithm>
#include <cpprest/http_msg.h>


std::shared_ptr<Request> Balancer::GetBest()
{
	std::vector<long> results;
	std::vector<std::shared_ptr<Request>> requests;
	for (auto s : config->getAddresses())
	{
		auto request = std::make_shared<Request>(s, L"/");
		requests.push_back(request);
		analyzer->setRequest(request);
		auto test = analyzer->invokeRepeatedRequest(L"GET", config->getTestCount(), config->getTestTimeDelta(), L"");
		try {
			test.wait();
		}
		catch (web::http::http_exception &e)
		{
			std::wcout << e.what() << std::endl;
			return request;
		}
		results.push_back(analyzer->getAvarageResponseTime());
	}

	int minIndex = std::distance(results.begin(), std::min_element(results.begin(), results.end()));
	return requests[minIndex];
}
