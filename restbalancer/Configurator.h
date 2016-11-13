#pragma once
#include <vector>
#include <chrono>

class Configurator
{
	std::vector<std::wstring> addresses;
	int testCount;
	std::chrono::milliseconds testTimeDelta;

public:
	std::vector<std::wstring> getAddresses() const;
	void setAddresses(const std::vector<std::wstring>& basic_wstrings);
	int getTestCount() const;
	void setTestCount(int test_count);
	std::chrono::milliseconds getTestTimeDelta() const;
	void setTestTimeDelta(const std::chrono::milliseconds& test_time_delta);
	void addAddress(std::wstring address);

	Configurator() {};
	Configurator(int count, std::chrono::milliseconds delta);


};
