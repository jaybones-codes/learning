#include "Simulation.h"
void Simulation::cleanupDeadEntities(EntityManager &em, ComponentManager &cm) {
  if (em.getDeadEntities().size() > 0) {

    for (auto e : em.getDeadEntities()) {
      cm.removeAllComponents(e);
      em.getActiveEntities().erase(e);
    }
    em.clearDeadEntities();
  }
}
