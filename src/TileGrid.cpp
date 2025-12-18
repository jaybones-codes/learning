#include "TileGrid.h"
#include <SDL3/SDL_render.h>
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
std::string TileGrid::tileTypeToString(TileType type) {
  switch (type) {
  case TileType::Empty:
    return "Empty";
  case TileType::Grass:
    return "Grass";
  case TileType::Wall:
    return "Wall";
  case TileType::Floor:
    return "Floor";
  case TileType::Water:
    return "Water";
  default:
    return "Unknown";
  }
}
void TileGrid::setTileColor(SDL_Renderer *renderer, TileType type) {
  switch (type) {
  case TileType::Empty:
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    break;
  case TileType::Grass:
    SDL_SetRenderDrawColor(renderer, 0, 128, 0, 255);
    break;
  case TileType::Wall:
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
    break;
  case TileType::Floor:
    SDL_SetRenderDrawColor(renderer, 138, 69, 19, 255);
    break;
  case TileType::Water:
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    break;
  default:
    TileType::Empty;
    break; // SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  }
}
void TileGrid::renderTileGrid(SDL_Renderer *renderer) { // No grid parameter!
  for (int y = 0; y < GRID_HEIGHT; y++) {               // Fixed bounds
    for (int x = 0; x < GRID_WIDTH; x++) {              // Fixed bounds
      TileType type = getTileType(x, y); // No grid->, just call directly
      setTileColor(renderer, type);      // No grid->

      int screenX = x * TILE_SIZE;
      int screenY = y * TILE_SIZE;

      SDL_FRect frect = {
          static_cast<float>(screenX), static_cast<float>(screenY),
          static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE)};
      SDL_RenderFillRects(renderer, &frect, 1);
    }
  }
}

// Green : rgba(0, 128, 0, 255)

//           Blue : rgba(0, 0, 255, 255)

//                     Brown : rgba(139, 69, 19, 255)

//                                  Grey : rgba(128, 128, 128, 255)
