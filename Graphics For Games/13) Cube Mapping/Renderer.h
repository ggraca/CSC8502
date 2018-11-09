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
  void DrawHeightMap();
  void DrawWater();
  void DrawSkybox();

  Shader* lightShader;
  Shader* reflectShader;
  Shader* skyboxShader;

  HeightMap* heightMap;
  Mesh* quad;

  Camera* camera;
  Light* light;

  GLuint cubeMap;
  float waterRotate;
};
