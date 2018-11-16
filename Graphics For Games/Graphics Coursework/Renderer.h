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

class Renderer : public OGLRenderer {
public:
  Renderer(Window &parent);
  virtual ~Renderer(void);

  virtual void UpdateScene(float msec);
  virtual void RenderScene();

protected:
  Camera* camera;

  // SceneGraph
  void BuildNodeLists(SceneNode* from);
  void SortNodeLists();
  void ClearNodeLists();
  void DrawNodes();
  void DrawNode(SceneNode* node);

  SceneNode* root;
  Frustum frameFrustum;
  vector<SceneNode*> transparentNodeList;
  vector<SceneNode*> nodeList;

  // Setup
  bool InstantiateShaders();
  bool InstantiateObjects();
  bool InstantiateLights();

  void SetupShadowFBO();
  bool SetupDeferredRenderingFBO();

  void DrawObjects();
  void DrawLights();
  void CombineBuffers();

  // Loop
  void DrawShadowScene();
  void DrawCombinedScene();
  void KeyboardShortcuts();

  // Utils
  void GenerateScreenTexture(GLuint &into, bool depth = false);
  void DefinePerspectives();
  void DrawSkybox();
  void DrawBloom();

  vector<Light*> lights;
  OBJMesh* sphere;
  Mesh* quad;

  Shader* sceneShader;
  Shader* shadowShader;
  Shader* combineShader;
  Shader* lightShader;
  Shader* skyboxShader;
  Shader* bloomShader;

  GLuint shadowFBO;
  GLuint shadowTex;

  GLuint objectFBO;
  GLuint objectColourTex;
  GLuint objectNormalTex;
  GLuint objectDepthTex;

  GLuint lightFBO;
  GLuint lightEmissiveTex;
  GLuint lightSpecularTex;

  GLuint combinedFBO;
  GLuint combinedColourTex;

  Matrix4 cameraPerspective;
  Matrix4 lightPerspective;
  Matrix4 orthPerspective;

  GLuint skybox;
};
