#pragma once
#include <SDL3/SDL.h>

class Entity {

public:
  Entity(float posX, float posY);

  virtual void update(float deltaTime) = 0;
  virtual void render(SDL_Renderer *renderer) = 0;
  virtual ~Entity() = 0;

  bool isActive() { return m_active; }
  float getPosX() { return m_posX; }
  float getPosY() { return m_posY; }
  float getSizeX() { return m_sizeX; }
  float getSizeY() { return m_sizeY; }

  void setActive(bool activity);

protected:
  float m_posX;
  float m_posY;
  float m_sizeX;
  float m_sizeY;

  bool m_active;
};
