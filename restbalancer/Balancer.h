#pragma once
#include "Configurator.h"
#include <memory>
#include <RequestAnalyzer.h>

class Balancer
{
	std::shared_ptr<Configurator> config;
	std::shared_ptr<RequestAnalyzer> analyzer;

public:
	Balancer(std::shared_ptr <Configurator> c, std::shared_ptr<RequestAnalyzer> a) : config(c), analyzer(a) {}
	std::shared_ptr<Request> GetBest();
};
