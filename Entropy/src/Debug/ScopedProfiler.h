#pragma once

#ifdef ENTROPY_DEBUG
class ScopedProfiler
{
public:
	ScopedProfiler(const char* functionName);
	virtual ~ScopedProfiler();

private:
	const char* m_FunctionName;
	std::chrono::steady_clock::time_point m_TimeBegin;
};

// Source: https://github.com/TheCherno/Hazel/blob/master/Hazel/src/Hazel/Debug/Instrumentor.h#L175
namespace ProfilerUtils 
{

	template <size_t N>
	struct ChangeResult
	{
		char Data[N];
	};

	template <size_t N, size_t K>
	constexpr auto CleanupOutputString(const char(&expr)[N], const char(&remove)[K])
	{
		ChangeResult<N> result = {};

		size_t srcIndex = 0;
		size_t dstIndex = 0;
		while (srcIndex < N)
		{
			size_t matchIndex = 0;
			while (matchIndex < K - 1 && srcIndex + matchIndex < N - 1 && expr[srcIndex + matchIndex] == remove[matchIndex])
			{
				matchIndex++;
			}
			if (matchIndex == K - 1)
			{
				srcIndex += matchIndex;
			}
			result.Data[dstIndex++] = expr[srcIndex] == '"' ? '\'' : expr[srcIndex];
			srcIndex++;
		}
		return result;
	}
}

#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
#define ENTROPY_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
#define ENTROPY_FUNC_SIG __PRETTY_FUNCTION__
#elif (defined(__FUNCSIG__) || (_MSC_VER))
#define ENTROPY_FUNC_SIG __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
#define ENTROPY_FUNC_SIG __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
#define ENTROPY_FUNC_SIG __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
#define ENTROPY_FUNC_SIG __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
#define ENTROPY_FUNC_SIG __func__
#else
#define ENTROPY_FUNC_SIG "ENTROPY_FUNC_SIG unknown!"
#endif

#define ENTROPY_PROFILE_SCOPE_LINE(name, line) \
	constexpr auto fixedName##line = ::ProfilerUtils::CleanupOutputString(name, "__cdecl "); \
	ScopedProfiler profiler##line(fixedName##line.Data)
#define ENTROPY_PROFILE_SCOPE(name) ENTROPY_PROFILE_SCOPE_LINE(name, __LINE__)
#define ENTROPY_PROFILE_FUNCTION() ENTROPY_PROFILE_SCOPE(ENTROPY_FUNC_SIG)

#else

#define ENTROPY_PROFILE_SCOPE_LINE(name, line)
#define ENTROPY_PROFILE_SCOPE(name)
#define ENTROPY_PROFILE_FUNCTION()

#endif // ENTROPY_DEBUG
