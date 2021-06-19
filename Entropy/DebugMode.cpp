#include "Precompiled.h"

#ifdef ENTROPY_DEBUG
DebugMode::DebugMode()
	: m_Mask(0)
	, m_Step(Step::NONE)
{

}

void DebugMode::SetStep(Step step)
{
	m_Step = step;
}

void DebugMode::Toggle(DebugOption option)
{
	m_Mask ^= 1 << (int)option;
}

Step DebugMode::GetStep() const
{
	return m_Step;
}

bool DebugMode::IsEnabled(DebugOption option) const
{
	return m_Mask & 1 << (int)option;
}
#endif // ENTROPY_DEBUG
