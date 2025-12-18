#include "TileGrid.h"
#include <iostream>
TileGrid::TileGrid() { generateGrid(); }
void TileGrid::generateGrid() {
  m_grid.resize(GRID_HEIGHT);
  for (std::vector<TileType> &row : m_grid) {
    row.resize(GRID_WIDTH, TileType::Empty);
  }
};
void TileGrid::setTileType(int y, int x, TileType type) {
  if (checkBounds(x, y)) {
    m_grid[y][x] = type;
  } else {
    std::cout << "Tile Out of bounds" << std::endl;
  }
};
bool TileGrid::checkBounds(int x, int y) {
  return x >= 0 && x < GRID_WIDTH && y >= 0 && y < GRID_HEIGHT;
}
