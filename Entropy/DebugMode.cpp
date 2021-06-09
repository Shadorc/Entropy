#ifdef _DEBUG
#include "Precompiled.h"

DebugMode::DebugMode()
	: m_debugMask(0)
{

}

void DebugMode::Enable(DebugOption option)
{
	m_debugMask ^= 1 << INT(option);
}

bool DebugMode::IsEnabled(DebugOption option) const
{
	return m_debugMask & 1 << INT(option);
}
#endif // _DEBUG