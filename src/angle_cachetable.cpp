#include "angle_cachetable.h"
void AngleCacheTable::Resize(int density)
{
    cache_table.resize(density);
    for (auto &row : cache_table)
    {
        row.resize(density, CacheState::Empty);
    }
}

void AngleCacheTable::ClearCache()
{
    for (auto &row : cache_table)
    {
        for (size_t i = 0; i < row.size(); i++)
        {
            row[i] = CacheState::Empty;
        }
    }
}

void AngleCacheTable::Activate(int u, int v)
{
    const int range = 1;
    for (int i = -range; i <= range; i++)
    {
        for (int j = -range; j <= range; j++)
        {
            int n_u = GetTablePosition(u + i);
            int n_v = GetTablePosition(v + j);

            if (cache_table[n_u][n_v] != CacheState::Cached)
            {
                cache_table[n_u][n_v] = CacheState::Activated;
            }
        }
    }
}

void AngleCacheTable::MarkCached(int u, int v)
{
    u = GetTablePosition(u);
    v = GetTablePosition(v);
    cache_table[u][v] = CacheState::Cached;
}

bool AngleCacheTable::IsCached(int u, int v) const
{
    u = GetTablePosition(u);
    v = GetTablePosition(v);
    return cache_table[u][v] == CacheState::Cached;
}

int AngleCacheTable::GetTablePosition(int coordinate) const
{
    const int size = cache_table.size();
    int result = 0;
    for (size_t i = 0; i < 2; i++)
    {
        if (coordinate >= size)
        {
            coordinate = coordinate - size;
        }
        else if (coordinate < 0)
        {
            coordinate = size + coordinate;
        }
        else
        {
            result = coordinate;
            break;
        }
    }
    return result;
}

bool AngleCacheTable::IsActivated(int u, int v) const
{
    u = GetTablePosition(u);
    v = GetTablePosition(v);
    return cache_table[u][v] == CacheState::Activated;
}
