#include "SpatialGrid.h"
#include <math.h>

SpatialGrid::SpatialGrid(float worldWidth, float worldHeight, float cellSize) {
  m_gridColumns = worldWidth / cellSize;
  m_gridRows = worldHeight / cellSize;

  m_grid.resize(m_gridRows);
  for (int i = 0; i < m_gridRows; i++) {
    m_grid[i].resize(m_gridColumns);
  }
}
void SpatialGrid::clear() { m_grid.clear(); }
void SpatialGrid::addEntity(Entity entity, float x, float y) {
  // convert screen to grid
  int cx = (int)(x / m_cellSize);
  int cy = (int)(y / m_cellSize);
  // clamp it
  cx = std::max(0, std::min(cx, m_gridColumns - 1));
  cy = std::max(0, std::min(cy, m_gridRows - 1));

  m_grid[cy][cx].push_back(entity);
}
std::vector<Entity> SpatialGrid::getNeighbors(float x, float y, float radius) {
  std::vector<Entity> neighbors;
  int cellX = x / m_cellSize;
  int cellY = y / m_cellSize;

  for (int offsetY = -1; offsetY <= 1; ++offsetY) {
    for (int offsetX = -1; offsetX <= 1; ++offsetX) {

      int nx = cellX + offsetX;
      int ny = cellY + offsetY;

      // bounds check
      if (nx < 0 || nx >= m_gridColumns)
        continue;
      if (ny < 0 || ny >= m_gridRows)
        continue;

      // iterate entities in neighbor cell
      for (Entity other : m_grid[ny][nx]) {
        neighbors.push_back(other);
      }
    }
  }
  return neighbors;
}
