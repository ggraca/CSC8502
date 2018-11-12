#include "Renderer.h"

Renderer::Renderer(Window &parent) : OGLRenderer(parent) {
  camera = new Camera(-40, 270, Vector3(0, 2000, 0));

  projMatrix = Matrix4::Perspective(
    1.0f, 10000.0f, (float)width / (float)height, 45.0f
  );

  if (!InstantiateShaders()) return;
  if (!InstantiateObjects()) return;

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  init = true;
}

Renderer::~Renderer(void) {
  delete root;
  delete camera;
  delete light;
}

bool Renderer::InstantiateShaders() {
  currentShader = new Shader(
    MYSHADERDIR"SceneVert.glsl",
    MYSHADERDIR"SceneFrag.glsl"
  );
  if (!currentShader->LinkProgram()) return false;

  return true;
}

bool Renderer::InstantiateObjects() {
  root = new SceneNode();

  Mesh* heightMap = new HeightMap(TEXTUREDIR"terrain2.raw");
  heightMap->SetTexture(
    SOIL_load_OGL_texture(TEXTUREDIR"brick.tga",
    SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS)
  );
  heightMap->SetBumpMap(SOIL_load_OGL_texture(
   TEXTUREDIR"brickDOT3.tga", SOIL_LOAD_AUTO,
   SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS
 ));
  if (!heightMap->GetTexture() || !heightMap->GetBumpMap()) return false;
  SetTextureRepeating(heightMap->GetTexture(), true);
  SetTextureRepeating(heightMap->GetBumpMap(), true);

  SceneNode* s = new SceneNode();
  s->SetColour(Vector4(1, 1, 1, 1));
  s->SetTransform(Matrix4::Translation(Vector3(
    RAW_WIDTH * HEIGHTMAP_X / -2, 0, RAW_HEIGHT * HEIGHTMAP_Z / -2
  )));
  s->SetModelScale(Vector3(1, 1, 1));
  s->SetBoundingRadius(1000000.0f);
  s->SetMesh(heightMap);
  root->AddChild(s);

  light = new Light(
    Vector3(0, 500, 0),
    Vector4(1, 1, 1, 1), RAW_WIDTH * HEIGHTMAP_X / 2.0f
  );

  return true;
}

void Renderer::UpdateScene(float msec) {
  camera->UpdateCamera(msec);
  viewMatrix = camera->BuildViewMatrix();
  frameFrustum.FromMatrix(projMatrix * viewMatrix);
  root->Update(msec);
}

void Renderer::RenderScene() {
  BuildNodeLists(root);
  SortNodeLists();

  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

  glUseProgram(currentShader->GetProgram());

  // Textures
  glUniform1i(
    glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0
  );
  // BumpMaps
  glUniform1i(
    glGetUniformLocation(currentShader->GetProgram(), "bumpTex"), 1
  );
  glUniform3fv(
    glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1,
    (float*)&camera->GetPosition()
  );

  UpdateShaderMatrices();
  SetShaderLight(*light);

  DrawNodes();

  glUseProgram(0);
  SwapBuffers();
  ClearNodeLists();
}

void Renderer::BuildNodeLists(SceneNode* from) {
  if (frameFrustum.InsideFrustum(*from)) {
    Vector3 dir =
      from->GetWorldTransform().GetPositionVector() -
      camera->GetPosition();

    from->SetCameraDistance(Vector3::Dot(dir, dir));

    if(from->GetColour().w < 1.0f)
      transparentNodeList.push_back(from);
    else
      nodeList.push_back(from);
  }

  for(auto i = from->GetChildIteratorStart();
      i != from->GetChildIteratorEnd(); i++)
    BuildNodeLists(*i);
}

void Renderer::SortNodeLists() {
  sort(
    transparentNodeList.begin(),
    transparentNodeList.end(),
    SceneNode::CompareByCameraDistance
  );

  sort(
    nodeList.begin(),
    nodeList.end(),
    SceneNode::CompareByCameraDistance
  );
}

void Renderer::DrawNodes() {
  for(auto n : nodeList) DrawNode(n);
  for(auto i = transparentNodeList.rbegin();
      i != transparentNodeList . rend (); ++ i ) {
    DrawNode(*i);
  }
}

void Renderer::DrawNode(SceneNode* node) {
  if (node->GetMesh()) {
    Matrix4 transform =
      node->GetWorldTransform() *
      Matrix4::Scale(node->GetModelScale());

    glUniformMatrix4fv(
      glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"),
      1, false,
      (float*)&transform
    );

    glUniform4fv(
      glGetUniformLocation(currentShader->GetProgram(), "nodeColour"),
      1, (float*)&node->GetColour()
    );

    glUniform1i(
      glGetUniformLocation(currentShader->GetProgram(), "useTexture"),
      (int)node->GetMesh()->GetTexture()
    );

    node->Draw(*this);
  }
}

void Renderer::ClearNodeLists() {
	transparentNodeList.clear();
	nodeList.clear();
}
