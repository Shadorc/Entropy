#include "Precompiled.h"

#ifdef ENTROPY_DEBUG
DebugMode::DebugMode()
	: m_debugMask(0)
{

}

void DebugMode::Enable(DebugOption option)
{
	m_debugMask ^= 1 << (int) option;
}

bool DebugMode::IsEnabled(DebugOption option) const
{
	return m_debugMask & 1 << (int) option;
}
#endif // ENTROPY_DEBUG
