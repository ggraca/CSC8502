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
#include <string>

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
  // SceneGraph
  void BuildNodeLists(SceneNode* from);
  void SortNodeLists();
  void ClearNodeLists();
  void DrawNodes(bool transparents = true);
  void DrawNode(SceneNode* node);

  // Setup
  bool SetupShaders();
  bool SetupFBOs();
  bool SetupShadowFBO();
  bool SetupObjectFBO();
  bool SetupLightFBO();
  bool SetupCombinedFBO();
  bool SetupBlurFBO();
  bool SetupBloomFBO();
  bool SetupUnderwaterFBO();
  bool BuildScenes();

  // Loop
  void DrawShadowScene();
  void DrawTerrain();
  void DrawObjects();
  void DrawGrass();
  void DrawWater();
  void DrawLights();
  void DrawSkybox();
  void DrawCombinedScene();
  void DrawBlur(GLuint colourTex);
  void DrawBloom(GLuint colourTex);
  void DrawUnderwater(GLuint colourTex);
  void PresentScene(GLuint colourTex);
  void KeyboardShortcuts();

  // Utils
  void LoadTexture(Mesh* into, string name);
  void LoadBumpMap(Mesh* into, string name);
  void GenerateScreenTexture(GLuint &into, bool depth = false);
  void GenerateShadowTexture(GLuint &into);
  void DefinePerspectives();
  void SelectSceneA();
  void SelectSceneB();
  void SelectSceneC();

  Camera* camera;
  int selectedCamera = 0;

  SceneNode* root;
  Frustum frameFrustum;
  vector<SceneNode*> transparentNodeList;
  vector<SceneNode*> nodeList;

  vector<Light*> lights;
  vector<int> activeLights;

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
  Shader* terrainShader;
  Shader* underwaterShader;
  Shader* grassShader;

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

  GLuint underwaterFBO;
  GLuint underwaterColourTex;

  Matrix4 cameraPerspective;
  Matrix4 orthPerspective;
  Matrix4 lightPerspective;
  Matrix4 sunPerspective;

  GLuint currentSkybox;
  GLuint skybox[3];

  OBJMesh* sphere;
  Mesh* quad;
  Mesh* water;
  Mesh* terrain;
  GLuint terrainTex[5];

  bool softShadows;
  bool bloom;
  bool grass;

  int currentScene;
  SceneNode* beams[4];
  MD5Node* hellknight;
};
