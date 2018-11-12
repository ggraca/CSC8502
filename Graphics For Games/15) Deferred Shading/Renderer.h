#pragma once

#include "../nclgl/OGLRenderer.h"
#include "../nclgl/Camera.h"
#include "../nclgl/OBJMesh.h"
#include "../nclgl/HeightMap.h"
#include "../nclgl/Light.h"

#define LIGHTNUM 8

class Renderer : public OGLRenderer {
public:
  Renderer(Window &parent);
  virtual ~Renderer(void);

  virtual void UpdateScene(float msec);
  virtual void RenderScene();

protected:
  void FillBuffers();
  void DrawPointLights();
  void CombineBuffers();
  void GenerateScreenTexture(GLuint &into, bool depth = false);

  Shader* sceneShader;
  Shader* pointLightShader;
  Shader* combineShader;

  Light* pointLights;
  Mesh* heightMap;
  Mesh* quad;
  OBJMesh* sphere;
  Camera* camera;

  float rotation;

  GLuint bufferFBO;
  GLuint bufferColourTex;
  GLuint bufferNormalTex;
  GLuint bufferDepthTex;

  GLuint pointLightFBO;
  GLuint lightEmissiveTex;
  GLuint lightSpecularTex;
};
