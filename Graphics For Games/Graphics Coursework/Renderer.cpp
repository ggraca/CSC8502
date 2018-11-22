#include "Renderer.h"

Renderer::Renderer(Window &parent) : OGLRenderer(parent) {
  quad = Mesh::GenerateQuad();
  root = new SceneNode();

  if (!SetupShaders()) return;
  if (!SetupFBOs()) return;
  if (!BuildScenes()) return;

  DefinePerspectives();

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
  delete water;
  delete terrain;

  for(auto l : lights) delete l;

  delete basicShader;
  delete sceneShader;
  delete shadowShader;
  delete combineShader;
  delete lightShader;
  delete skyboxShader;
  delete blurShader;
  delete bloomFilterShader;
  delete bloomCombineShader;
  delete waterShader;
  delete terrainShader;
  delete underwaterShader;
  delete grassShader;
  currentShader = NULL;

  glDeleteFramebuffers(1, &shadowFBO);
  glDeleteFramebuffers(1, &objectFBO);
  glDeleteFramebuffers(1, &lightFBO);
  glDeleteFramebuffers(1, &combinedFBO);
  glDeleteFramebuffers(1, &blurFBO);
  glDeleteFramebuffers(1, &bloomFBO);
  glDeleteFramebuffers(1, &underwaterFBO);

  for(int i = 0; i < MAX_SHADOWS; i++)
    glDeleteTextures(1, &shadowTex[i]);
  glDeleteTextures(1, &objectColourTex);
  glDeleteTextures(1, &objectNormalTex);
  glDeleteTextures(1, &objectDepthTex);
  glDeleteTextures(1, &lightEmissiveTex);
  glDeleteTextures(1, &lightSpecularTex);
  glDeleteTextures(1, &combinedColourTex);
  for(int i = 0; i < 2; i++)
    glDeleteTextures(1, &blurColourTex[i]);
  glDeleteTextures(1, &bloomColourTex);
  glDeleteTextures(1, &underwaterColourTex);
  for(int i = 0; i < 5; i++)
    glDeleteTextures(1, &terrainTex[i]);
}

void Renderer::UpdateScene(float msec) {
  deltaTime += msec;
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
  DrawTerrain();
  DrawObjects();
  if (grass) DrawGrass();
  DrawWater();
  glBindFramebuffer(GL_FRAMEBUFFER, lightFBO);
  DrawLights();
  glBindFramebuffer(GL_FRAMEBUFFER, combinedFBO);
  DrawSkybox();
  DrawCombinedScene();

  // Post Processing
  projMatrix = orthPerspective;
  viewMatrix.ToIdentity();
  UpdateShaderMatrices();

  glBindFramebuffer(GL_FRAMEBUFFER, underwaterFBO);
  DrawUnderwater(combinedColourTex);

  if (bloom) {
    glBindFramebuffer(GL_FRAMEBUFFER, bloomFBO);
    DrawBloom(underwaterColourTex);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    PresentScene(bloomColourTex);
  } else {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    PresentScene(underwaterColourTex);
  }

  glUseProgram(0);
  SwapBuffers();
  KeyboardShortcuts();
}
