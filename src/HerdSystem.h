#pragma once
#include <cstdint>
#include <unordered_map>
#include <vector>
// forward declarations only
struct PositionComponent;
struct VelocityComponent;
struct HerdComponent;

using Entity = uint32_t;

struct SpatialGrid {
  float cellSize;
  std::unordered_map<int, std::unordered_map<int, std::vector<Entity>>> cells;

  explicit SpatialGrid(float size);
  void clear();
  void insert(Entity e, float x, float y);
};

class HerdSystem {
public:
  explicit HerdSystem(float cellSize);

  void update(float deltaTime,
              std::unordered_map<Entity, PositionComponent> &positions,
              std::unordered_map<Entity, VelocityComponent> &velocities,
              std::unordered_map<Entity, HerdComponent> &herds);

private:
  SpatialGrid grid;
};
