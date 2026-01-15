#pragma once
#include "ECS.h"

class Simulation {
public:
  Simulation() {}
  ~Simulation() {}

  void cleanupDeadEntities(EntityManager &em, ComponentManager &cm);

private:
};
