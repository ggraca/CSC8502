#pragma once

#include <algorithm>
#include "../nclgl/OGLRenderer.h"
#include "../nclgl/SceneNode.h"
#include "../nclgl/Frustum.h"
#include "../nclgl/Camera.h"
#include "../nclgl/Light.h"
#include "../nclgl/HeightMap.h"
#include "../nclgl/MD5Mesh.h"
#include "../nclgl/MD5Node.h"
#include "../nclgl/OBJMesh.h"

#define LIGHTNUM 2
#define SHADOWSIZE 2048
#define POST_PASSES 50
#define MAX_SHADOWS 5

class Renderer : public OGLRenderer {
public:
  Renderer(Window &parent);
  virtual ~Renderer(void);

  virtual void UpdateScene(float msec);
  virtual void RenderScene();

protected:
  Camera* camera;
  int selectedCamera = 0;

  // SceneGraph
  void BuildNodeLists(SceneNode* from);
  void SortNodeLists();
  void ClearNodeLists();
  void DrawNodes(bool transparents = true);
  void DrawNode(SceneNode* node);

  SceneNode* root;
  Frustum frameFrustum;
  vector<SceneNode*> transparentNodeList;
  vector<SceneNode*> nodeList;

  // Setup
  bool SetupShaders();
  bool SetupFBOs();
  bool SetupShadowFBO();
  bool SetupObjectFBO();
  bool SetupLightFBO();
  bool SetupCombinedFBO();
  bool SetupBlurFBO();
  bool SetupBloomFBO();

  bool BuildSceneA();
  void SelectSceneA();
  void SelectSceneB();

  void DrawObjects();
  void DrawWater();
  void DrawLights();
  void CombineBuffers();

  // Loop
  void DrawShadowScene();
  void DrawCombinedScene();
  void KeyboardShortcuts();

  // Utils
  void GenerateScreenTexture(GLuint &into, bool depth = false);
  void GenerateShadowTexture(GLuint &into);
  void DefinePerspectives();
  void DrawSkybox();
  void DrawBlur(GLuint colourTex);
  void DrawBloom(GLuint colourTex);
  void PresentScene(GLuint colourTex);

  vector<Light*> lights;
  vector<int> activeLights;
  OBJMesh* sphere;
  Mesh* quad;

  Shader* basicShader;
  Shader* sceneShader;
  Shader* shadowShader;
  Shader* combineShader;
  Shader* lightShader;
  Shader* skyboxShader;
  Shader* blurShader;
  Shader* bloomFilterShader;
  Shader* bloomCombineShader;
  Shader* waterShader;

  GLuint shadowFBO;
  GLuint shadowTex[5];
  Matrix4 shadowMatrix[5];

  GLuint objectFBO;
  GLuint objectColourTex;
  GLuint objectNormalTex;
  GLuint objectDepthTex;

  GLuint lightFBO;
  GLuint lightEmissiveTex;
  GLuint lightSpecularTex;

  GLuint combinedFBO;
  GLuint combinedColourTex;

  GLuint blurFBO;
  GLuint blurColourTex[2];

  GLuint bloomFBO;
  GLuint bloomColourTex;

  Matrix4 cameraPerspective;
  Matrix4 orthPerspective;
  Matrix4 lightPerspective;
  Matrix4 sunPerspective;

  GLuint currentSkybox;
  GLuint skybox[2];

  Mesh* water;
  Mesh* terrain;

  bool softShadows;
  bool bloom;
  bool rotateLight;
};
