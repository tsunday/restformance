#include "stdafx.h"
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <map>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include "Configurator.h"
#include "RequestAnalyzer.h"
#include "Balancer.h"

using namespace std;

using namespace utility;
using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

#define CONSOLE(a) wcout << a << endl;

std::map<string_t, string_t> dictionary;
std::shared_ptr<Request> bestService;

void handle_get(http_request request)
{
	std::wcout << "Got GET" << std::endl;
	bestService->setResource(request.relative_uri().to_string());

	string_t body;
	request.extract_string().then([&body](pplx::task<string_t> task)
	{
		body = task.get();
	}).wait();
	auto task = bestService->invokeRequest(L"GET", body);
	task.wait();

	request.reply(task.get().status_code(), task.get().body());
}

void usage()
{
	CONSOLE("usage: restbalancer testCount testTimeDelta url1 url2 [...]")
	CONSOLE("testCount        -  number of test requests")
	CONSOLE("testTimeDelta    -  time delay between requests")
	CONSOLE("urlX             -  rest service address")
}

void parse_error()
{
	CONSOLE("Argument parse error or invalid argument list")
		usage();
}

int _tmain(int argc, _TCHAR* argv[])
{
	std::wstring address = L"http://localhost:8080/balancer";
	http_listener listener(address);

	if (argc == 1)
	{
		usage();
		return 0;
	}
	if (wcscmp(argv[1], L"-h") == 0) {
		usage();
		return 0;
	}

	if (argc < 4)
	{
		parse_error();
		return 1;
	}

	auto config = std::make_shared<Configurator>(wcstol(argv[1], nullptr, 10), chrono::milliseconds(wcstol(argv[2], nullptr, 10)));
	auto analyzer = std::make_shared<RequestAnalyzer>();
	Balancer balancer(config, analyzer);

	for(int i = 3; i < argc; i++)
	{
		config->addAddress(argv[i]);
	}

	bestService = balancer.GetBest();

	std::wcout << "Resolved best service: " << bestService->getUrl() << std::endl;

	listener.support(methods::GET, handle_get);

	try
	{
		listener
			.open()
			.then([&listener, &address]() { std::wcout << "Balancer started at " << address << std::endl;})
			.wait();

		while (true);
	}
	catch (std::exception const & e)
	{
		std::wcout << e.what() << std::endl;
	}

	return 0;
}

