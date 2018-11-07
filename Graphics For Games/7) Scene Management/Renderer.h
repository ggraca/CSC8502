#pragma once

#include <algorithm>
#include "../nclgl/OGLRenderer.h"
#include "../nclgl/Camera.h"
#include "../nclgl/SceneNode.h"
#include "../nclgl/Frustum.h"
#include "CubeRobot.h"

class Renderer : public OGLRenderer {
public:
  Renderer(Window &parent);
  virtual ~Renderer(void);

  virtual void UpdateScene(float msec);
  virtual void RenderScene();

protected:
  void BuildNodeLists(SceneNode* from);
  void SortNodeLists();
  void ClearNodeLists();
  void DrawNodes();
  void DrawNode(SceneNode* node);

  SceneNode* root;
  Camera* camera;
  Mesh* quad;

  Frustum frameFrustum;
  vector<SceneNode*> transparentNodeList;
  vector<SceneNode*> nodeList;
};
