#pragma once
#include <cstdint>
#include <unordered_map>
struct PositionComponent;
struct VelocityComponent;
struct BoidComponent;
struct Vec2;
using Entity = uint32_t;
#include "SpatialGrid.h"
struct BoidSpecs {
  // Global Boid Specs
  //
  //  Weights
  float cohesionWeight;
  float separationWeight;
  float alignmentWeight;
  // Distances
  float separationRadius;
  float alignmentRadius;
  float cohesionRadius;
  // MaxSpeed
  float maxSpeed;
  float maxForce;
  // Boundary Behaviour
  float boundaryRadius;
  float boundaryWeight;
};
class BoidSystem {
private:
  SpatialGrid m_spatialGrid;
  std::vector<Entity> m_neighbours;
  float m_worldWidth = 1000;
  float m_worldHeight = 1000;
  BoidSpecs m_specs;

public:
  BoidSystem(float worldWidth, float worldHeight, float cellSize);
  BoidSpecs &getSpecs() { return m_specs; }
  void update(float deltaTime, std::unordered_map<Entity, BoidComponent> &boids,
              std::unordered_map<Entity, PositionComponent> &positions,
              std::unordered_map<Entity, VelocityComponent> &velocities);
};

Vec2 limit(Vec2 vec, float max);
Vec2 normalize(Vec2 vec);
