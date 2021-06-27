#pragma once

#ifdef ENTROPY_DEBUG
enum class DebugOption
{
	SHOW_COMMANDS,
	PERFORMANCE_INFO,
	SHOW_QUADTREE,
	SHOW_AABB,
	SHOW_VELOCITY,
	COUNT
};

enum class PhysicStep
{
	CONTINUE,
	PAUSE,
	NONE
};

class DebugMode
{
private:
	int m_Mask;
	PhysicStep m_PhysicStep;

public:
	DebugMode();

	void TogglePhysicStep();
	void ContinuePhysicStep();
	bool ShouldContinuePhysicStep();

	void Toggle(DebugOption option);
	bool IsEnabled(DebugOption option) const;
};
#endif // ENTROPY_DEBUG
