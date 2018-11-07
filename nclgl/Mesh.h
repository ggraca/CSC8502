#pragma once
#include "OGLRenderer.h"

enum MeshBuffer {
  VERTEX_BUFFER,
  COLOUR_BUFFER,
  TEXTURE_BUFFER,
  INDEX_BUFFER,
  MAX_BUFFER
};

class Mesh {
public:
  Mesh(void);
  ~Mesh(void);

  virtual void Draw();
  static Mesh* GenerateTriangle();
  static Mesh* GenerateQuad();

  void SetTexture(GLuint tex) { texture = tex; }
  GLuint GetTexture() { return texture; }

protected:
  void BufferData();

  GLuint arrayObject;
  GLuint bufferObject[MAX_BUFFER];
  GLuint numVertices;
  GLuint type;

  Vector3* vertices;
  Vector4* colours;

  GLuint texture;
  Vector2* textureCoords;

  GLuint numIndices;
  unsigned int* indices;
};
