#ifndef ANGLE_CACHETABLE_CLASS_DEF
#define ANGLE_CACHETABLE_CLASS_DEF
#include <vector>
class AngleCacheTable
{
public:
    void Resize(int density);
    void ClearCache();
    void Activate(int u, int v);
    void MarkCached(int u, int v);
    bool IsCached(int u, int v) const;
    bool IsActivated(int u, int v) const;

protected:
    int GetTablePosition(int coordinate) const;
    enum class CacheState
    {
        Empty,
        Activated,
        Cached,
    };
    std::vector<std::vector<CacheState>> cache_table;
};

#endif