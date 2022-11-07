#include "Precompiled.h"

#ifdef ENTROPY_DEBUG
constexpr long long WARNING_THRESHOLD = 4000; // 4ms

ScopedProfiler::ScopedProfiler(const char* functionName)
	: m_FunctionName(functionName)
	, m_TimeBegin(std::chrono::steady_clock::now())
{
}

ScopedProfiler::~ScopedProfiler()
{
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	long long us = std::chrono::duration_cast<std::chrono::microseconds>(end - m_TimeBegin).count();
	if (us > WARNING_THRESHOLD)
	{
		ENTROPY_LOG(m_FunctionName << ": " << us << "us");
	}
}
#endif // ENTROPY_DEBUG