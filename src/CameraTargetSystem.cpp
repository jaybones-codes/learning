#include "CameraTargetSystem.h"

void CameraTargetSystem::update(
    Camera &camera, std::unordered_map<Entity, PositionComponent> &positions,
    std::unordered_map<Entity, CameraTargetComponent> &targets) {

  for (auto &[entity, target] : targets) {
    if (positions.count(entity)) {

      float camX = positions[entity].x - (camera.getViewportWidth() / 2);
      float camY = positions[entity].y - (camera.getViewportHeight() / 2);
      camera.setX(camX);
      camera.setY(camY);
    }
  }
};
