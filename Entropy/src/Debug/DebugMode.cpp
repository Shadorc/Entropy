#include "Precompiled.h"

#ifdef ENTROPY_DEBUG
DebugMode::DebugMode()
	: m_Mask(0)
	, m_PhysicStep(PhysicStep::NONE)
{

}

void DebugMode::TogglePhysicStep()
{
	m_PhysicStep = (m_PhysicStep == PhysicStep::NONE) ? PhysicStep::PAUSE : PhysicStep::NONE;
}

void DebugMode::ContinuePhysicStep()
{
	m_PhysicStep = PhysicStep::CONTINUE;
}

bool DebugMode::ShouldContinuePhysicStep()
{
	if (m_PhysicStep == PhysicStep::NONE)
	{
		return true;
	}
	if (m_PhysicStep == PhysicStep::CONTINUE)
	{
		m_PhysicStep = PhysicStep::PAUSE;
		return true;
	}
	return false;
}

void DebugMode::Toggle(DebugOption option)
{
	m_Mask ^= 1 << static_cast<int>(option);
}

bool DebugMode::IsEnabled(DebugOption option) const
{
	return m_Mask & 1 << static_cast<int>(option);
}
#endif // ENTROPY_DEBUG
