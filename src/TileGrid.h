#pragma once
#include <SDL3/SDL.h>
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
  const int GRID_WIDTH = 50;
  const int GRID_HEIGHT = 50;

  void generateGrid();
  void renderTileGrid(SDL_Renderer *renderer, float cameraX, float cameraY);
  void renderLineGrid(SDL_Renderer *renderer, float cameraX, float cameraY);
  void setTileType(int x, int y, TileType type);
  TileType getTileType(int x, int y);
  void tileHighlight(SDL_Renderer *renderer, int mouseX, int mouseY,
                     float cameraX, float cameraY);
  void setTileColor(SDL_Renderer *renderer, TileType type);
  void renderBrushIndicator(SDL_Renderer *renderer);
  void setCurrentBrush(TileType type);
  TileType getCurrentBrush() const;

private:
  std::vector<std::vector<TileType>> m_grid;
  bool checkBounds(int x, int y);
  int hoveredTileX = -1;
  int hoveredTileY = -1;
  TileType currentBrush = TileType::Wall;
};
