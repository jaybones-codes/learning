#pragma once

#include "ECS.h"
#include "TileGrid.h"
//*************************************************************************************************
//
//
//  SYSTEMS FOR THE ECS, ALL IN ONE PLACE, EVENTUALLY
//  ALL NEW SYSTEMS SHOULD BE ADDED HERE!!!
//
//
//*************************************************************************************************

struct CollisionComponent; // FORWARD DECLARATION, LIVES IN ECS.h

class CollisionSystem {
public:
  void update(TileGrid &tg,
              std::unordered_map<Entity, PositionComponent> &positions,
              std::unordered_map<Entity, CollisionComponent> &collisions,
              std::unordered_map<Entity, VelocityComponent> &velocities);

  bool checkTileCollision(float x, float y, float w, float h, TileGrid &tg);
};
