#include "BoidSystem.h"
#include "ECS.h"

#include <cmath>
void BoidSystem::update(
    float deltaTime, std::unordered_map<Entity, BoidComponent> &boids,
    std::unordered_map<Entity, PositionComponent> &positions,
    std::unordered_map<Entity, VelocityComponent> &velocities) {

  for (auto &[entity, boid] : boids) {
    if (!positions.count(entity) || !velocities.count(entity))
      continue;

    auto &pos = positions[entity];
    auto &vel = velocities[entity];

    Vec2 separation =
        calculateSeparation(entity, vel, pos, boid, positions, boids);
    Vec2 alignment = calculateAlignment(entity, pos, vel, boid, positions,
                                        boids, velocities);
    Vec2 cohesion = calculateCohesion(entity, pos, vel, boid, positions, boids);

    separation.x *= boid.separationWeight;
    separation.y *= boid.separationWeight;
    alignment.x *= boid.alignmentWeight;
    alignment.y *= boid.alignmentWeight;
    cohesion.x *= boid.cohesionWeight;
    cohesion.y *= boid.cohesionWeight;

    Vec2 force = {separation.x + alignment.x + cohesion.x,
                  separation.y + alignment.y + cohesion.y};
    force = limit(force, boid.maxForce);

    vel.vx += force.x * deltaTime;
    vel.vy += force.y * deltaTime;
    // Boundary avoidance
    float margin = 100.0f;
    float turnForce = 200.0f;

    if (pos.x < margin) {
      vel.vx += turnForce * deltaTime;
    }
    if (pos.x > 800 - margin) { // Use your actual world width
      vel.vx -= turnForce * deltaTime;
    }
    if (pos.y < margin) {
      vel.vy += turnForce * deltaTime;
    }
    if (pos.y > 600 - margin) { // Use your actual world height
      vel.vy -= turnForce * deltaTime;
    }
    // Limit velocity
    Vec2 velVec{vel.vx, vel.vy};
    velVec = limit(velVec, boid.maxSpeed);
    vel.vx = velVec.x;
    vel.vy = velVec.y;
  }
}

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
      steerX += dx / (distance * distance);
      steerY += dy / (distance * distance);
      count++;
    }
  }

  if (count > 0) {
    steerX /= count;
    steerY /= count;
  }
  Vec2 steer{steerX, steerY};
  float mag = sqrt(steerX * steerX + steerY * steerY);
  if (mag > 0) {

    normalize(steer);
    steer.x *= entityBoid.maxSpeed;
    steer.y *= entityBoid.maxSpeed;

    steer.x -= entityVelocity.vx;
    steer.y -= entityVelocity.vy;

    steer = limit(steer, entityBoid.maxForce);
  }
  return steer;
}
Vec2 BoidSystem::calculateAlignment(
    Entity entity, PositionComponent &entityPosition,
    VelocityComponent &entityVelocity, BoidComponent &entityBoid,
    std::unordered_map<Entity, PositionComponent> &positions,
    std::unordered_map<Entity, BoidComponent> &boids,
    std::unordered_map<Entity, VelocityComponent> &velocities) {
  float steerX = 0.0f;
  float steerY = 0.0f;
  int count = 0;
  for (auto &[otherEntity, otherBoid] : boids) {
    if (otherEntity == entity)
      continue;
    if (!positions.count(otherEntity))
      continue;
    if (!velocities.count(otherEntity))
      continue;

    auto &otherPosition = positions[otherEntity];
    auto &otherVelocity = velocities[otherEntity];

    auto dx = entityPosition.x - otherPosition.x;
    auto dy = entityPosition.y - otherPosition.y;
    auto distance = std::sqrt(dx * dx + dy * dy);

    if (distance > 0 && distance < entityBoid.alignmentRadius) {
      steerX += otherVelocity.vx;
      steerY += otherVelocity.vy;
      count++;
    }
  }
  if (count > 0) {
    steerX /= count;
    steerY /= count;
  }
  Vec2 steer{steerX, steerY};
  float mag = sqrt(steerX * steerX + steerY * steerY);
  if (mag > 0) {

    normalize(steer);
    steer.x *= entityBoid.maxSpeed;
    steer.y *= entityBoid.maxSpeed;

    steer.x -= entityVelocity.vx;
    steer.y -= entityVelocity.vy;

    steer = limit(steer, entityBoid.maxForce);
  }
  return steer;
}

Vec2 BoidSystem::calculateCohesion(
    Entity entity, PositionComponent &entityPosition,
    VelocityComponent &entityVelocity, BoidComponent &entityBoid,
    std::unordered_map<Entity, PositionComponent> &positions,
    std::unordered_map<Entity, BoidComponent> &boids) {
  Vec2 center{0.0f, 0.0f};
  int count = 0;

  for (auto &[otherEntity, otherBoid] : boids) {
    if (otherEntity == entity)
      continue;
    if (!positions.count(otherEntity))
      continue;

    auto &otherPos = positions[otherEntity];

    float dx = otherPos.x - entityPosition.x;
    float dy = otherPos.y - entityPosition.y;
    float dist = std::sqrt(dx * dx + dy * dy);

    if (dist > 0.0f && dist < entityBoid.cohesionRadius) {
      center.x += otherPos.x;
      center.y += otherPos.y;
      count++;
    }
  }

  if (count == 0)
    return {0.0f, 0.0f};

  // Average center
  center.x /= count;
  center.y /= count;

  // Desired direction toward center
  Vec2 steer{center.x - entityPosition.x, center.y - entityPosition.y};

  // Convert to steering force
  steer = normalize(steer);
  steer.x *= entityBoid.maxSpeed;
  steer.y *= entityBoid.maxSpeed;

  // Steering = desired − current velocity
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
