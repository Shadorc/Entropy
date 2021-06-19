#pragma once

#ifdef ENTROPY_DEBUG
enum class DebugOption
{
	PERFORMANCE_INFO,
	SHOW_QUADTREE,
	SHOW_AABB,
	SHOW_VELOCITY,
	COUNT
};

enum class Step
{
	CONTINUE,
	PAUSE,
	NONE
};

class DebugMode
{
private:
	int m_Mask;
	Step m_Step;

public:
	DebugMode();

	void SetStep(Step step);
	void Toggle(DebugOption option);

	Step GetStep() const;
	bool IsEnabled(DebugOption option) const;
};
#endif // ENTROPY_DEBUG
