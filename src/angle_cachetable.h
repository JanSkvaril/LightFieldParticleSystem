#ifndef ANGLE_CACHETABLE_CLASS_DEF
#define ANGLE_CACHETABLE_CLASS_DEF
#include <vector>

// Chaching table for generator
class AngleCacheTable
{
public:
    // Resize and reset table
    void Resize(int density);
    // Reset table to inicial state
    void ClearCache();
    // activate record at (u,v) coordiantes (will also activate surounding records)
    void Activate(int u, int v);
    // mark record at (u, v) coordintates as cached
    void MarkCached(int u, int v);
    // returns True if record is marked as cached
    bool IsCached(int u, int v) const;
    // returns True if record is marked as cached
    bool IsActivated(int u, int v) const;

protected:
    // returns position of given coordinate (includes over and underflows)
    int GetTablePosition(int coordinate) const;

    enum class CacheState
    {
        Empty,     // record has not been generated, and should not be generated
        Activated, // record has not been generated, but should be generated
        Cached,    // record has been generated
    };
    // cache table
    std::vector<std::vector<CacheState>> cache_table;
};

#endif