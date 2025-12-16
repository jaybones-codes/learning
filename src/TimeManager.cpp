#include "TimeManager.h"
#include <iostream>

TimeManager::TimeManager() {

  m_lastFrameTime =
      Clock::now(); // Init the last frame so it's not a weird random number;
}

void TimeManager::updateDeltaTime() {

  // Figure out DeltaTime
  auto currentTime = Clock::now();
  auto duration = currentTime - m_lastFrameTime;
  m_deltaTime = std::chrono::duration<float>(duration).count();
  m_lastFrameTime = currentTime;
  //
  m_frameCount++;
  m_fpsUpdateTimer += m_deltaTime;
  if (m_fpsUpdateTimer >= 1.0f) {
    m_fps = m_frameCount / m_fpsUpdateTimer;

    m_frameCount = 0;
    m_fpsUpdateTimer = 0.0f;
  }
};
