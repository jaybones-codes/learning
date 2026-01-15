#pragma once
#include "ECS.h"

class Simulation {
public:
  Simulation() {}
  ~Simulation() {}

  void cleanupDeadEntities(EntityManager &em, ComponentManager &cm);
  // Globals
  int SCREENWIDTH = 800;
  int SCREENHEIGHT = 600;

private:
};
