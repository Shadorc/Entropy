#ifdef _DEBUG
#pragma once

enum class DebugOption
{
    PERFORMANCE_INFO,
    SHOW_QUADTREE,
    SHOW_AABB,
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
#endif // _DEBUG
