// restformance_cli.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "RequestFactory.h"
#include "AnalyzeController.h"
using namespace std;

#define CONSOLE(a) wcout << a << endl;

void usage()
{
    CONSOLE("usage: restformance_cli url resource method test [concurrent [count [delay [body]]]]")
    CONSOLE("url        -  rest service address")
    CONSOLE("resource   -  service resource")
    CONSOLE("method     -  request method (GET | POST | PUT | DELETE)")
    CONSOLE("test       -  selected test (Responsiveness | Concurrency")
    CONSOLE("count      -  request repeat quantity (default: 20)")
    CONSOLE("delay      -  time delay between requests [ms] (default: 500ms)")
    CONSOLE("concurrent -  number of concurrent requests - for Concurrency test only (default: 2)")
    CONSOLE("body       -  request body (json)")
    CONSOLE("filename   -  output results filename (default: results*.csv")
}

void parse_error()
{
    CONSOLE("Argument parse error or invalid argument list")
    usage();
}


int _tmain(int argc, _TCHAR* argv[])
{
    string_t address;
    string_t resource;
    method mtd;
    TestType test_type;
    string_t body;
    string_t filename = L"results";
    int request_count = 20;
    milliseconds delta(500);
    int concurrent = 2;

    if (argc == 1)
    {
        usage();
        return 0;
    }
    if (wcscmp(argv[1], L"-h") == 0){
        usage();
        return 0;
    }

    if ( argc < 5)
    {
        parse_error();
        return 1;
    }
 
    address = argv[1];
    resource = argv[2];
 
    wstring wmtd = argv[3];
    if (wmtd.compare(L"GET") == 0)
        mtd = methods::GET;
    else if (wmtd.compare(L"POST") == 0)
        mtd = methods::POST;
    else if (wmtd.compare(L"PUT") == 0)
        mtd = methods::PUT;
    else if (wmtd.compare(L"DELETE") == 0)
        mtd = methods::DEL;
    else
    {
        parse_error();
        return 1;
    }

    wstring wtest = argv[4];
    if (wtest.compare(L"Responsiveness") == 0)
        test_type = TestType::Responsiveness;
    else if (wtest.compare(L"Concurrency") == 0)
        test_type = TestType::Concurrency;
    else
    {
        parse_error();
        return 1;
    }

    if (argc > 5)
        request_count = wcstol(argv[5], nullptr, 10);
    if (argc > 6)
        delta = milliseconds(wcstol(argv[6], nullptr, 10));
    if (argc > 7)
        concurrent = wcstol(argv[7], nullptr, 10);
    if (argc > 8)
        body = argv[8];
    if (argc > 9)
        filename = argv[9];


    CONSOLE("Starting analyze")

    auto controller = AnalyzeController(mtd, request_count, delta, body, filename);
    for (int i = 0; i < concurrent; i++){
        controller.addRequest(address, resource);
        if (test_type != TestType::Concurrency) break;
    }
    controller.performTest(test_type);

    return 0;
}

