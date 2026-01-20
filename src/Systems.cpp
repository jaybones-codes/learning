#include "Systems.h"
#include "helpers.h"
#include <iostream>
void CollisionSystem::update(
    TileGrid &tg, std::unordered_map<Entity, PositionComponent> &positions,
    std::unordered_map<Entity, CollisionComponent> &collisions,
    std::unordered_map<Entity, VelocityComponent> &velocities) {
  // check tile collision
  for (auto &[entity1, position] : positions) {
    if (!positions.contains(entity1))
      continue;

    auto &pos1 = positions[entity1];
    auto &col1 = collisions[entity1];
    if (checkTileCollision(pos1.x, pos1.y, col1.w, col1.h, tg)) {
    }
  }

  // check entity collision
  for (auto &[entity1, collision] : collisions) {

    if (!positions.contains(entity1))
      continue;
    auto &pos1 = positions[entity1];
    auto &col1 = collisions[entity1];
    auto &vel1 = velocities[entity1];
    for (auto &[entity2, collision] : collisions) {
      if (entity1 == entity2)
        continue;
      if (!positions.contains(entity2))
        continue;
      auto &pos2 = positions[entity2];
      auto &col2 = collisions[entity2];
      if (checkAABB(pos1.x, pos1.y, col1.w, col1.h, pos2.x, pos2.y, col2.w,
                    col2.h)) {
        vel1.vx = -vel1.vx;
        vel1.vy = -vel1.vy;
        // std::cout << "Collision!!!!!!" << std::endl;
      }
    }
  }
}
bool CollisionSystem::checkTileCollision(float x, float y, float w, float h,
                                         TileGrid &tg) {
  int tileLeft = x / tg.TILE_SIZE;
  int tileRight = (x + w) / tg.TILE_SIZE;
  int tileTop = y / tg.TILE_SIZE;
  int tileBottom = (y + h) / tg.TILE_SIZE;

  for (int ty = tileTop; ty <= tileBottom; ty++) {
    for (int tx = tileLeft; tx <= tileRight; tx++) {
      if (!tg.isWalkable(tx, ty))
        std::cout << "Tile is NOT walkable" << std::endl;
      return true;
    }
  }
  std::cout << "Tile IS walkable" << std::endl;
  return false;
}
