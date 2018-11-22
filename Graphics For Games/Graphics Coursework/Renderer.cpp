#include "Renderer.h"

Renderer::Renderer(Window &parent) : OGLRenderer(parent) {
  quad = Mesh::GenerateQuad();
  root = new SceneNode();

  if (!SetupShaders()) return;
  if (!SetupFBOs()) return;
  if (!BuildSceneA()) return;

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

  for (auto l : lights) delete l;

  delete shadowShader;
  delete sceneShader;
  delete combineShader;
  delete lightShader;
  delete skyboxShader;
  currentShader = NULL;

  for(int i = 0; i < MAX_SHADOWS; i++)
    glDeleteTextures(1, &shadowTex[i]);
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
  deltaTime += msec;
  camera->UpdateCamera(msec);
  frameFrustum.FromMatrix(cameraPerspective * viewMatrix);
  root->Update(msec);

  if (rotateLight) {
    Vector3 currentPos = lights[0]->GetPosition();
    lights[0]->SetPosition(Vector3(currentPos.x, currentPos.y, currentPos.z + (msec / 20)));
  }
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
  DrawGrass();
  DrawWater();
  glBindFramebuffer(GL_FRAMEBUFFER, lightFBO);
  DrawLights();
  glBindFramebuffer(GL_FRAMEBUFFER, combinedFBO);
  DrawSkybox();
  CombineBuffers();

  // Post Processing
  projMatrix = orthPerspective;
  viewMatrix.ToIdentity();
  UpdateShaderMatrices();

  if (bloom) {
    glBindFramebuffer(GL_FRAMEBUFFER, bloomFBO);
    DrawBloom(combinedColourTex);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    PresentScene(bloomColourTex);
  } else {
    glBindFramebuffer(GL_FRAMEBUFFER, underwaterFBO);
    DrawUnderwater(combinedColourTex);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    PresentScene(underwaterColourTex);
  }


  glUseProgram(0);
  SwapBuffers();
  KeyboardShortcuts();
}
