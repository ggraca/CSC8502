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

void Renderer::LoadTexture(Mesh* into, string name) {
  into->SetTexture(SOIL_load_OGL_texture(
    (string(MYTEXTUREDIR) + name).c_str(), SOIL_LOAD_AUTO,
    SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS)
  );
  SetTextureRepeating(into->GetTexture(), true);
  glBindTexture(GL_TEXTURE_2D, into->GetTexture());
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::LoadBumpMap(Mesh* into, string name) {
  into->SetBumpMap(SOIL_load_OGL_texture(
    (string(MYTEXTUREDIR) + name).c_str(), SOIL_LOAD_AUTO,
    SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS
  ));
  SetTextureRepeating(into->GetBumpMap(), true);
  glBindTexture(GL_TEXTURE_2D, terrain->GetBumpMap());
  glGenerateMipmap(GL_TEXTURE_2D);
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

  if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_1))
    SelectSceneA();
  if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_2))
    SelectSceneB();
  if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_3))
    SelectSceneC();

  if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_RIGHT)) {
    currentScene++;
    if (currentScene > 2) currentScene = 0;

    if (currentScene == 0) SelectSceneA();
    if (currentScene == 1) SelectSceneB();
    if (currentScene == 2) SelectSceneC();
  }
  if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_LEFT)) {
    currentScene--;
    if (currentScene < 0) currentScene = 2;

    if (currentScene == 0) SelectSceneA();
    if (currentScene == 1) SelectSceneB();
    if (currentScene == 2) SelectSceneC();
  }
}
