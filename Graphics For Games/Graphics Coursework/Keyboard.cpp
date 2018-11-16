#include "Renderer.h"

void Renderer::KeyboardShortcuts() {
  if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_1)) {
    camera->SetPosition(lights[0]->GetPosition());
  }
}
