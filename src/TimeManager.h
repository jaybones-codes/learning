#pragma once
#include <chrono>

class TimeManager {

public:
  TimeManager();
  void updateDeltaTime();
  float getDeltaTime() { return m_deltaTime; };
  float getFPS() { return m_fps; };
  void setTargetFPS(float fps) { m_targetFPS = fps; };

private:
  // NEW: Time tracking
  using Clock = std::chrono::high_resolution_clock;
  using TimePoint = Clock::time_point;

  TimePoint m_lastFrameTime = Clock::now();
  float m_deltaTime = 0.0f;
  float m_fps = 0.0f;
  float m_targetFPS = 60.0f;

  // FPS calculation
  int m_frameCount = 0;
  float m_fpsUpdateTimer = 0.0f;
};
