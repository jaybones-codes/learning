#pragma once

#include <SDL3/SDL.h>
#include <array>
#include <unordered_map>

enum class ButtonState {
  Up,      // Not pressed
  Pressed, // Just pressed this frame
  Down,    // Held down
  Released // Just released this frame
};

class Input {
public:
  Input() = default;

  // Update input state (call once per frame BEFORE processing events)
  void update();

  // Process SDL events (call from Engine::pollEvents)
  void processEvent(const SDL_Event &event);

  // Keyboard queries
  bool isKeyDown(SDL_Scancode key) const;
  bool isKeyPressed(SDL_Scancode key) const;
  bool isKeyReleased(SDL_Scancode key) const;

  // Mouse queries
  bool isMouseButtonDown(int button) const;
  bool isMouseButtonPressed(int button) const;
  bool isMouseButtonReleased(int button) const;

  int getMouseX() const { return m_mouseX; }
  int getMouseY() const { return m_mouseY; }

  // Utility functions
  float getAxis(SDL_Scancode negative, SDL_Scancode positive) const;

  // Helper to get/set button state
  ButtonState getKeyState(SDL_Scancode key) const;
  void setKeyState(SDL_Scancode key, ButtonState state);
  float getMouseX() { return m_mouseX; }
  float getMouseY() { return m_mouseY; }

private:
  // Keyboard state
  std::unordered_map<SDL_Scancode, ButtonState> m_keyStates;

  // Mouse state
  std::array<ButtonState, 5> m_mouseButtons = {ButtonState::Up};
  int m_mouseX = 0;
  int m_mouseY = 0;
};
