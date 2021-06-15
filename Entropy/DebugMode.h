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

class DebugMode
{
private:
    int m_debugMask;

public:
    DebugMode();

    void Toggle(DebugOption option);
    bool IsEnabled(DebugOption option) const;
};
#endif // ENTROPY_DEBUG
