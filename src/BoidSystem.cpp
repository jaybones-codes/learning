#include "BoidSystem.h"
#include "ECS.h"

#include <cmath>

Vec2 BoidSystem::calculateSeparation(
    Entity entity, VelocityComponent &entityVelocity,
    PositionComponent &entityPosition, BoidComponent &entityBoid,
    std::unordered_map<Entity, PositionComponent> &positions,
    std::unordered_map<Entity, BoidComponent> &boids) {
  float steerX = 0.0f;
  float steerY = 0.0f;
  int count = 0;

  for (auto &[otherEntity, otherBoid] : boids) {
    if (otherEntity == entity)
      continue;
    if (!positions.count(otherEntity))
      continue;

    auto &otherPosition = positions[otherEntity];

    float dx = entityPosition.x - otherPosition.x;
    float dy = entityPosition.y - otherPosition.y;
    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance > 0.0f && distance < entityBoid.separationRadius) {
      steerX += dx / distance;
      steerY += dy / distance;
      count++;
    }
  }

  if (count > 0) {
    steerX /= count;
    steerY /= count;
  }
  Vec2 steer{steerX, steerY};

  normalize(steer);
  steer.x *= entityBoid.maxSpeed;
  steer.y *= entityBoid.maxSpeed;

  steer.x -= entityVelocity.vx;
  steer.y -= entityVelocity.vy;

  steer = limit(steer, entityBoid.maxForce);

  return steer;
}

float getDistance(PositionComponent a, PositionComponent b) {

  float dx = a.x - b.x;
  float dy = a.y - b.y;
  return std::sqrt(dx * dx + dy * dy);
}
Vec2 normalize(Vec2 vec) {
  float magnitude = sqrt(vec.x * vec.x + vec.y * vec.y);
  if (magnitude > 0) {
    vec.x /= magnitude;
    vec.y /= magnitude;
  }
  return vec;
}
Vec2 limit(Vec2 vec, float max) {
  float magnitude = sqrt(vec.x * vec.x + vec.y * vec.y);
  if (magnitude > max) {
    vec.x = (vec.x / magnitude) * max;
    vec.y = (vec.y / magnitude) * max;
  }
  return vec;
}
