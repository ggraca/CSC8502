#pragma once

#include "../nclgl/OGLRenderer.h"
#include "../nclgl/Camera.h"
#include "../nclgl/HeightMap.h"
#include "../nclgl/Light.h"

class Renderer : public OGLRenderer {
public:
  Renderer(Window &parent);
  virtual ~Renderer(void);

  virtual void UpdateScene(float msec);
  virtual void RenderScene();

protected:
  Mesh* heightMap;
  Camera* camera;
  Light* light;
};
