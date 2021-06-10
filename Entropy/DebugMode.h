#ifdef ENTROPY_DEBUG
#pragma once

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

    void Enable(DebugOption option);
    bool IsEnabled(DebugOption option) const;
};
#endif // ENTROPY_DEBUG
