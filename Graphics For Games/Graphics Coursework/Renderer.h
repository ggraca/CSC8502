#pragma once

#include <algorithm>
#include "../nclgl/OGLRenderer.h"
#include "../nclgl/Camera.h"
#include "../nclgl/SceneNode.h"
#include "../nclgl/Frustum.h"
#include "../nclgl/HeightMap.h"
#include "../nclgl/CubeRobot.h"

class Renderer : public OGLRenderer {
public:
  Renderer(Window &parent);
  virtual ~Renderer(void);

  virtual void UpdateScene(float msec);
  virtual void RenderScene();

protected:
  bool InstantiateShaders();
  bool InstantiateObjects();
  void BuildNodeLists(SceneNode* from);
  void SortNodeLists();
  void ClearNodeLists();
  void DrawNodes();
  void DrawNode(SceneNode* node);

  SceneNode* root;
  Camera* camera;
  Light* light;

  Frustum frameFrustum;
  vector<SceneNode*> transparentNodeList;
  vector<SceneNode*> nodeList;
};
