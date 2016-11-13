#include "stdafx.h"
#include "Configurator.h"
#include <iterator>
#include <iostream>
#include <sstream>

std::vector<std::wstring> Configurator::getAddresses() const
{
	return addresses;
}

void Configurator::setAddresses(const std::vector<std::wstring>& basic_wstrings)
{
	addresses = basic_wstrings;
}

int Configurator::getTestCount() const
{
	return testCount;
}

void Configurator::setTestCount(int test_count)
{
	testCount = test_count;
}

std::chrono::milliseconds Configurator::getTestTimeDelta() const
{
	return testTimeDelta;
}

void Configurator::setTestTimeDelta(const std::chrono::milliseconds& test_time_delta)
{
	testTimeDelta = test_time_delta;
}

void Configurator::addAddress(std::wstring address)
{
	addresses.push_back(address);
}


Configurator::Configurator(int count, std::chrono::milliseconds delta) : testCount(count), testTimeDelta(delta)
{
}
