#include "Precompiled.h"

#ifdef ENTROPY_DEBUG
DebugMode::DebugMode()
	: m_Mask(0)
{

}

void DebugMode::Toggle(DebugOption option)
{
	m_Mask ^= 1 << (int) option;
}

bool DebugMode::IsEnabled(DebugOption option) const
{
	return m_Mask & 1 << (int) option;
}
#endif // ENTROPY_DEBUG
