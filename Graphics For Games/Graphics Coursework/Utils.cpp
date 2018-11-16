#include "Renderer.h"

void Renderer::GenerateScreenTexture(GLuint &into, bool depth) {
  glGenTextures(1, &into);
  glBindTexture(GL_TEXTURE_2D, into);

  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  glTexImage2D(
    GL_TEXTURE_2D, 0,
    depth ? GL_DEPTH_COMPONENT24 : GL_RGBA8,
    width, height, 0,
    depth ? GL_DEPTH_COMPONENT : GL_RGBA,
    GL_UNSIGNED_BYTE, NULL
  );

  glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::DefinePerspectives() {
  cameraPerspective = Matrix4::Perspective(
    1.0f, 15000.0f, (float)width / (float)height, 45.0f
  );
  lightPerspective = Matrix4::Perspective(
    325.0f, 15000.0f, (float)width / (float)height, 45.0f
  );
  orthPerspective = Matrix4::Orthographic(-1, 1, 1, -1, -1, 1);
}

void Renderer::KeyboardShortcuts() {
  if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_1)) {
    camera->SetPosition(lights[0]->GetPosition());
  }
}
