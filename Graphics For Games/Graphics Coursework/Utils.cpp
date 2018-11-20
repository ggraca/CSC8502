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

void Renderer::GenerateShadowTexture(GLuint &into) {
  glGenTextures(1, &into);
  glBindTexture(GL_TEXTURE_2D, into);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  glTexImage2D(
    GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOWSIZE,
    SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL
  );

  glTexParameteri(
    GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE
  );

  glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::DefinePerspectives() {
  lightPerspective = Matrix4::Perspective(
    1.0f, 10000.0f, (float)width / (float)height, 90.0f
  );
  sunPerspective = Matrix4::Perspective(
    1.0f, 10000.0f, (float)width / (float)height, 45.0f
  );
  orthPerspective = Matrix4::Orthographic(-1, 1, 1, -1, -1, 1);
  cameraPerspective = Matrix4::Perspective(
    1.0f, 10000.0f, (float)width / (float)height, 45.0f
  );
}

void Renderer::KeyboardShortcuts() {
  if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_TAB)) {
    selectedCamera++;
    if (selectedCamera >= activeLights.size()) selectedCamera = 0;
    camera->SetPosition(lights[activeLights[selectedCamera]]->GetPosition());
  }

  if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_1)) {
    SelectSceneA();
  }
  if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_2)) {
    SelectSceneB();
  }
  if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_3)) {
    ;
  }
}
