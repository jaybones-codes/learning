#pragma once
#include "Camera.h"
#include "ECS.h"
#include <cstdint>

struct PositionComponent;
using Entity = uint32_t;
class CameraTargetSystem {
public:
  void update(Camera &camera,
              std::unordered_map<Entity, PositionComponent> &positions,
              std::unordered_map<Entity, CameraTargetComponent> &targets);
};
