#pragma once
#include <utility>
#include <vector>

enum class TileType {
  Empty = 0,
  Grass = 1,
  Wall = 3,
  Floor = 4,
  Water = 5,

};
class TileGrid {

public:
  TileGrid();
  const int TILE_SIZE = 32;
  const int GRID_WIDTH = 20;
  const int GRID_HEIGHT = 20;

  void generateGrid();
  TileType getTileType(int x, int y) { return m_grid[y][x]; }

  void setTileType(int y, int x, TileType type);
  bool checkBounds(int x, int y);

private:
  std::vector<std::vector<TileType>> m_grid;
};
