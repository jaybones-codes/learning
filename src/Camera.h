#pragma once

class Input;

class Camera {
public:
  Camera(float viewportWidth, float viewportHeight, float worldWidth,
         float worldHeight);

  float getX() const { return m_x; }
  float getY() const { return m_y; }

  void update(float deltaTime, const Input &input);
  void worldToScreen(float worldX, float worldY, float &screenX,
                     float &screenY) const;

private:
  float m_x;
  float m_y;
  float m_speed;
  float m_viewportWidth;
  float m_viewportHeight;
  float m_worldWidth;
  float m_worldHeight;
};
