
#include "SpatialGrid.h"
#include <algorithm>
#include <cmath>

// Constructor
SpatialGrid::SpatialGrid(float worldWidth, float worldHeight, float cellSize)
    : m_cellSize(cellSize) {
  // Use ceil to avoid edge out-of-bounds
  m_gridColumns = static_cast<int>(std::ceil(worldWidth / m_cellSize));
  m_gridRows = static_cast<int>(std::ceil(worldHeight / m_cellSize));

  m_grid.resize(m_gridRows);
  for (int y = 0; y < m_gridRows; ++y) {
    m_grid[y].resize(m_gridColumns);
  }
}

// Clear cells, NOT the grid itself
void SpatialGrid::clear() {
  for (auto &row : m_grid) {
    for (auto &cell : row) {
      cell.clear();
    }
  }
}

// Insert entity into grid
void SpatialGrid::addEntity(Entity entity, float x, float y) {
  int cx = static_cast<int>(x / m_cellSize);
  int cy = static_cast<int>(y / m_cellSize);

  // Clamp indices
  cx = std::clamp(cx, 0, m_gridColumns - 1);
  cy = std::clamp(cy, 0, m_gridRows - 1);

  m_grid[cy][cx].push_back(entity);
}

// Get neighboring entities (distance check handled elsewhere)
std::vector<Entity> SpatialGrid::getNeighbors(float x, float y,
                                              float /*radius*/) {
  std::vector<Entity> neighbors;

  int cellX = static_cast<int>(x / m_cellSize);
  int cellY = static_cast<int>(y / m_cellSize);

  cellX = std::clamp(cellX, 0, m_gridColumns - 1);
  cellY = std::clamp(cellY, 0, m_gridRows - 1);

  for (int offsetY = -1; offsetY <= 1; ++offsetY) {
    for (int offsetX = -1; offsetX <= 1; ++offsetX) {
      int nx = cellX + offsetX;
      int ny = cellY + offsetY;

      if (nx < 0 || nx >= m_gridColumns)
        continue;
      if (ny < 0 || ny >= m_gridRows)
        continue;

      for (Entity other : m_grid[ny][nx]) {
        neighbors.push_back(other);
      }
    }
  }

  return neighbors;
}
