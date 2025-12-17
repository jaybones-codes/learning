#include "Input.h"
#include <iostream>
void Input::update() {
  // Keyboard
  for (auto &[key, state] : m_keyStates) {
    if (state == ButtonState::Pressed) {
      state = ButtonState::Down;
    } else if (state == ButtonState::Released) {
      state = ButtonState::Up;
    }
  }

  // Mouse buttons
  for (auto &state : m_mouseButtons) {
    if (state == ButtonState::Pressed) {
      state = ButtonState::Down;
    } else if (state == ButtonState::Released) {
      state = ButtonState::Up;
    }
  }
}

void Input::processEvent(const SDL_Event &event) {
  switch (event.type) {
  case SDL_EVENT_KEY_DOWN: {
    SDL_Scancode scanCode = event.key.scancode;
    auto state = m_keyStates.find(scanCode);

    if (state == m_keyStates.end() || state->second == ButtonState::Up) {
      m_keyStates[scanCode] = ButtonState::Pressed;
    }
    break;
  }
  case SDL_EVENT_KEY_UP:
    m_keyStates[event.key.scancode] = ButtonState::Released;
    break;

  case SDL_EVENT_MOUSE_BUTTON_DOWN: {
    int index = event.button.button - 1;
    if (index >= 0 && index < 5) {
      m_mouseButtons[index] = ButtonState::Pressed;
    }
    break;
  }

  case SDL_EVENT_MOUSE_BUTTON_UP: {
    int index = event.button.button - 1;
    if (index >= 0 && index < 5) {
      m_mouseButtons[index] = ButtonState::Released;
    }
    break;
  }

  case SDL_EVENT_MOUSE_MOTION:
    m_mouseX = event.motion.x;
    m_mouseY = event.motion.y;
    std::cout << m_mouseX << " " << m_mouseY << std::endl;
    break;
  }
}
bool Input::isKeyDown(SDL_Scancode key) const {

  ButtonState state = getKeyState(key);
  if (state == ButtonState::Down || state == ButtonState::Pressed) {
    return true;
  }
  return false;
}

bool Input::isKeyPressed(SDL_Scancode key) const {
  ButtonState state = getKeyState(key);
  if (state == ButtonState::Pressed) {
    return true;
  }
  return false;
}

bool Input::isKeyReleased(SDL_Scancode key) const {
  ButtonState state = getKeyState(key);
  if (state == ButtonState::Released) {
    return true;
  }
  return false;
}
bool Input::isMouseButtonDown(int button) const {

  if (button < 0 || button >= 5) {
    return false;
  }
  if (m_mouseButtons[button] == ButtonState::Down ||
      m_mouseButtons[button] == ButtonState::Pressed) {
    return true;
  }
  return false;
}

bool Input::isMouseButtonPressed(int button) const {
  if (button < 0 || button >= 5)
    return false;
  return m_mouseButtons[button] == ButtonState::Pressed;
}

bool Input::isMouseButtonReleased(int button) const {
  if (button < 0 || button >= 5)
    return false;
  return m_mouseButtons[button] == ButtonState::Released;
}

ButtonState Input::getKeyState(SDL_Scancode key) const {
  auto it = m_keyStates.find(key);
  if (it != m_keyStates.end()) {
    return it->second;
  };
  return ButtonState::Up;
}
void Input::setKeyState(SDL_Scancode key, ButtonState state) {

  m_keyStates[key] = state;
}
float Input::getAxis(SDL_Scancode negative, SDL_Scancode positive) const {
  float value = 0.0f;

  if (isKeyDown(positive)) {
    value += 1.0f;
  }
  if (isKeyDown(negative)) {
    value -= 1.0f;
  }

  return value;
}
