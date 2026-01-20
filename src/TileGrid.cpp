#include "TileGrid.h"
#include <iostream>

TileGrid::TileGrid() { generateGrid(); }

void TileGrid::generateGrid() {
  m_grid = std::vector<std::vector<TileType>>(
      GRID_HEIGHT, std::vector<TileType>(GRID_WIDTH, TileType::Floor));
}
bool TileGrid::isWalkable(int x, int y) {
  TileType type = getTileType(x, y);
  if (type == TileType::Wall || type == TileType::Water) {
    return false;
  } else {
    return true;
  }
}
bool TileGrid::checkBounds(int x, int y) {
  return x >= 0 && x < GRID_WIDTH && y >= 0 && y < GRID_HEIGHT;
}

void TileGrid::setTileType(int x, int y, TileType type) {
  if (checkBounds(x, y)) {
    m_grid[y][x] = type;
  }
}

TileType TileGrid::getTileType(int x, int y) {
  if (checkBounds(x, y)) {
    return m_grid[y][x];
  }
  return TileType::Empty;
}

void TileGrid::setTileColor(SDL_Renderer *renderer, TileType type) {
  switch (type) {
  case TileType::Floor:
    SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);
    break;
  case TileType::Wall:
    SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
    break;
  case TileType::Water:
    SDL_SetRenderDrawColor(renderer, 50, 120, 200, 255);
    break;
  case TileType::Grass:
    SDL_SetRenderDrawColor(renderer, 80, 160, 60, 255);
    break;
  case TileType::Empty:
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    break;
  }
}

void TileGrid::renderTileGrid(SDL_Renderer *renderer, float cameraX,
                              float cameraY) {
  for (int y = 0; y < GRID_HEIGHT; y++) {
    for (int x = 0; x < GRID_WIDTH; x++) {
      TileType tile = m_grid[y][x];

      float worldX = x * TILE_SIZE;
      float worldY = y * TILE_SIZE;
      float screenX = worldX - cameraX;
      float screenY = worldY - cameraY;

      setTileColor(renderer, tile);

      SDL_FRect rect = {screenX, screenY, (float)TILE_SIZE, (float)TILE_SIZE};
      SDL_RenderFillRect(renderer, &rect);
    }
  }
}

void TileGrid::renderLineGrid(SDL_Renderer *renderer, float cameraX,
                              float cameraY) {
  SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);

  for (int y = 0; y <= GRID_HEIGHT; y++) {
    float lineY = y * TILE_SIZE - cameraY;
    SDL_RenderLine(renderer, -cameraX, lineY, GRID_WIDTH * TILE_SIZE - cameraX,
                   lineY);
  }

  for (int x = 0; x <= GRID_WIDTH; x++) {
    float lineX = x * TILE_SIZE - cameraX;
    SDL_RenderLine(renderer, lineX, -cameraY, lineX,
                   GRID_HEIGHT * TILE_SIZE - cameraY);
  }
}

void TileGrid::tileHighlight(SDL_Renderer *renderer, int mouseX, int mouseY,
                             float cameraX, float cameraY) {
  int tileX = mouseX / TILE_SIZE;
  int tileY = mouseY / TILE_SIZE;

  if (checkBounds(tileX, tileY)) {
    hoveredTileX = tileX;
    hoveredTileY = tileY;
  } else {
    hoveredTileX = -1;
    hoveredTileY = -1;
  }

  if (hoveredTileX >= 0 && hoveredTileY >= 0) {
    float worldX = hoveredTileX * TILE_SIZE;
    float worldY = hoveredTileY * TILE_SIZE;
    float screenX = worldX - cameraX;
    float screenY = worldY - cameraY;

    SDL_FRect rect = {screenX, screenY, (float)TILE_SIZE, (float)TILE_SIZE};
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderRect(renderer, &rect);
  }
}
void TileGrid::renderBrushIndicator(SDL_Renderer *renderer) {
  setTileColor(renderer, currentBrush);
  SDL_FRect rect = {10, 10, 40, 40};
  SDL_RenderFillRect(renderer, &rect);

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_FRect outline = {10, 10, 40, 40};
  SDL_RenderRect(renderer, &outline);
}

void TileGrid::setCurrentBrush(TileType type) { currentBrush = type; }

TileType TileGrid::getCurrentBrush() const { return currentBrush; }
