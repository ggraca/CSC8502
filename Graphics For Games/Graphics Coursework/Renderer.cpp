#include "Renderer.h"

Renderer::Renderer(Window &parent) : OGLRenderer(parent) {
  camera = new Camera(-20, 270, Vector3(-80, 20, 0));

  DefinePerspectives();
  if (!InstantiateShaders()) return;
  if (!InstantiateObjects()) return;
  if (!InstantiateLights()) return;

  SetupShadowFBO();
  if (!SetupDeferredRenderingFBO()) return;

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

  init = true;
}

Renderer::~Renderer(void) {
  delete root;
  delete camera;

  delete quad;
  delete sphere;

  for (auto l : lights) delete l;

  delete shadowShader;
  delete sceneShader;
  delete combineShader;
  delete lightShader;
  delete skyboxShader;
  currentShader = NULL;

  glDeleteTextures(1, &shadowTex);
  glDeleteTextures(1, &objectColourTex);
  glDeleteTextures(1, &objectNormalTex);
  glDeleteTextures(1, &objectDepthTex);
  glDeleteTextures(1, &lightEmissiveTex);
  glDeleteTextures(1, &lightSpecularTex);

  glDeleteFramebuffers(1, &shadowFBO);
  glDeleteFramebuffers(1, &objectFBO);
  glDeleteFramebuffers(1, &lightFBO);
}

void Renderer::UpdateScene(float msec) {
  camera->UpdateCamera(msec);
  frameFrustum.FromMatrix(cameraPerspective * viewMatrix);
  root->Update(msec);
}

void Renderer::RenderScene() {
  ClearNodeLists();
  BuildNodeLists(root);
  SortNodeLists();

  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

  // Pre Processing
  glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
  DrawShadowScene();

  // Render Scene
  glBindFramebuffer(GL_FRAMEBUFFER, objectFBO);
  DrawObjects();
  glBindFramebuffer(GL_FRAMEBUFFER, lightFBO);
  DrawLights();
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  DrawSkybox();
  CombineBuffers();

  // Post Processing
  // glBindFramebuffer(GL_FRAMEBUFFER, 0);
  // DrawBloom();

  SwapBuffers();
  KeyboardShortcuts();
}
