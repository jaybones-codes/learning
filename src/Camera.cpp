#include "Camera.h"
#include "Input.h"
#include <SDL3/SDL_scancode.h>
#include <algorithm>

Camera::Camera(float viewportWidth, float viewportHeight, float worldWidth,
               float worldHeight)
    : m_x(0), m_y(0), m_speed(300.0f), m_viewportWidth(viewportWidth),
      m_viewportHeight(viewportHeight), m_worldWidth(worldWidth),
      m_worldHeight(worldHeight) {}

void Camera::update(float deltaTime, const Input &input) {
  if (input.isKeyDown(SDL_SCANCODE_A)) {
    m_x -= m_speed * deltaTime;
  }
  if (input.isKeyDown(SDL_SCANCODE_D)) {
    m_x += m_speed * deltaTime;
  }
  if (input.isKeyDown(SDL_SCANCODE_W)) {
    m_y -= m_speed * deltaTime;
  }
  if (input.isKeyDown(SDL_SCANCODE_S)) {
    m_y += m_speed * deltaTime;
  }

  // Clamp to world bounds
  m_x = std::max(0.0f, std::min(m_x, m_worldWidth - m_viewportWidth));
  m_y = std::max(0.0f, std::min(m_y, m_worldHeight - m_viewportHeight));
}

void Camera::worldToScreen(float worldX, float worldY, float &screenX,
                           float &screenY) const {
  screenX = worldX - m_x;
  screenY = worldY - m_y;
}
