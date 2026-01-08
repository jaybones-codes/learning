

#include "BoidSystem.h"
#include "ECS.h"
#include <algorithm>
#include <cmath>

BoidSystem::BoidSystem(float worldWidth, float worldHeight, float cellSize)
    : m_worldWidth(worldWidth), m_worldHeight(worldHeight),
      m_spatialGrid(worldWidth, worldHeight, cellSize) {
  m_neighbours.reserve(128);
  // Default boid specs
  m_specs.cohesionWeight = 1.0f;
  m_specs.separationWeight = 3.5f;
  m_specs.alignmentWeight = 1.0f;

  m_specs.separationRadius = 40.0f;
  m_specs.alignmentRadius = 60.0f;
  m_specs.cohesionRadius = 80.0f;
  m_specs.maxSpeed = 120.0f;
  m_specs.maxForce = 200.0f;

  m_specs.boundaryRadius = 100.0f;
  m_specs.boundaryWeight = 4.0f;
}

void BoidSystem::update(
    float dt, std::unordered_map<Entity, BoidComponent> &boids,
    std::unordered_map<Entity, PositionComponent> &positions,
    std::unordered_map<Entity, VelocityComponent> &velocities) {

  m_spatialGrid.clear();
  for (auto &[entity, pos] : positions) {
    m_spatialGrid.addEntity(entity, pos.x, pos.y);
  }

  for (auto &[entity, boid] : boids) {

    auto posIt = positions.find(entity);
    auto velIt = velocities.find(entity);
    if (posIt == positions.end() || velIt == velocities.end())
      continue;

    PositionComponent &pos = posIt->second;
    VelocityComponent &vel = velIt->second;

    float sepR = m_specs.separationRadius;
    float aliR = m_specs.alignmentRadius;
    float cohR = m_specs.cohesionRadius;

    float sepRSq = sepR * sepR;
    float aliRSq = aliR * aliR;
    float cohRSq = cohR * cohR;

    float maxR = std::max(sepR, std::max(aliR, cohR));

    m_spatialGrid.getNeighbors(pos.x, pos.y, m_neighbours);

    Vec2 sep{0, 0};
    Vec2 ali{0, 0};
    Vec2 coh{0, 0};

    int sepCount = 0;
    int aliCount = 0;
    int cohCount = 0;

    for (Entity other : m_neighbours) {
      if (other == entity)
        continue;

      auto &otherPos = positions[other];
      auto &otherVel = velocities[other];

      float dx = otherPos.x - pos.x;
      float dy = otherPos.y - pos.y;
      float distSq = dx * dx + dy * dy;

      if (distSq == 0.0f || distSq > maxR * maxR)
        continue;

      // --- Distance-weighted Separation ---
      if (distSq < sepRSq) {
        float minDistSq = 16.0f; // 4px minimum
        float inv = 1.0f / std::max(distSq, minDistSq);
        sep.x -= dx * inv;
        sep.y -= dy * inv;
        sepCount++;
      }

      // Alignment
      if (distSq < aliRSq) {
        ali.x += otherVel.vx;
        ali.y += otherVel.vy;
        aliCount++;
      }

      // Cohesion
      if (distSq < cohRSq) {
        coh.x += otherPos.x;
        coh.y += otherPos.y;
        cohCount++;
      }
    }

    Vec2 steer{0, 0};

    if (sepCount > 0) {
      normalize(sep);
      steer.x += sep.x * m_specs.separationWeight;
      steer.y += sep.y * m_specs.separationWeight;
    }

    if (aliCount > 0) {
      ali.x /= aliCount;
      ali.y /= aliCount;
      normalize(ali);
      steer.x += ali.x * m_specs.alignmentWeight;
      steer.y += ali.y * m_specs.alignmentWeight;
    }

    if (cohCount > 0) {
      coh.x /= cohCount;
      coh.y /= cohCount;
      coh.x -= pos.x;
      coh.y -= pos.y;
      normalize(coh);
      steer.x += coh.x * m_specs.cohesionWeight;
      steer.y += coh.y * m_specs.cohesionWeight;
    }

    // --- Boundary avoidance (stronger, scaled by maxForce) ---
    Vec2 boundary{0.0f, 0.0f};
    float r = m_specs.boundaryRadius;
    float turnForce = m_specs.maxForce; // scale to maxForce

    if (pos.x < r)
      boundary.x = (turnForce * (r - pos.x) / r);
    else if (pos.x > m_worldWidth - r)
      boundary.x = -(turnForce * (pos.x - (m_worldWidth - r)) / r);

    if (pos.y < r)
      boundary.y = (turnForce * (r - pos.y) / r);
    else if (pos.y > m_worldHeight - r)
      boundary.y = -(turnForce * (pos.y - (m_worldHeight - r)) / r);

    steer.x += boundary.x;
    steer.y += boundary.y;

    // Then limit total force
    steer = limit(steer, m_specs.maxForce);
    // Apply force
    steer = limit(steer, m_specs.maxForce);

    vel.vx += steer.x * dt;
    vel.vy += steer.y * dt;

    Vec2 v{vel.vx, vel.vy};
    v = limit(v, m_specs.maxSpeed);
    vel.vx = v.x;
    vel.vy = v.y;
  }
}
Vec2 normalize(Vec2 vec) {
  float mag = std::sqrt(vec.x * vec.x + vec.y * vec.y);
  if (mag > 0.0001f) {
    vec.x /= mag;
    vec.y /= mag;
  }
  return vec;
}

Vec2 limit(Vec2 vec, float max) {
  float mag = std::sqrt(vec.x * vec.x + vec.y * vec.y);
  if (mag > max) {
    vec.x = (vec.x / mag) * max;
    vec.y = (vec.y / mag) * max;
  }
  return vec;
}
