#pragma once
#include "ECS.h"
#include <cstdint>
#include <unordered_map>

struct PositionComponent;
struct VelocityComponent;
struct BoidComponent;
struct Vec2;
using Entity = uint32_t;

class PlayerInputSystem {
public:
  void update(std::unordered_map<Entity, PlayerInputComponent> &playerInputs,
              std::unordered_map<Entity, VelocityComponent> &velocities);
};
