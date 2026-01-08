#pragma once
#include <cstdint>
#include <vector>

using Entity = uint32_t;
class SpatialGrid {
private:
  float m_cellSize;
  int m_gridRows;
  int m_gridColumns;
  std::vector<std::vector<std::vector<Entity>>> m_grid;
  // m_grid[y][x] = list of entities

public:
  SpatialGrid(float worldWidth, float worldHeight, float cellSize);
  void clear();
  void addEntity(Entity entity, float x, float y);
  void getNeighbors(float x, float y, std::vector<Entity> &out);
};
