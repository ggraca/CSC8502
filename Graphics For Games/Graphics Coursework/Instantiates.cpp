#include "Renderer.h"

bool Renderer::InstantiateShaders() {
  shadowShader = new Shader(
    MYSHADERDIR"ShadowVert.glsl",
    MYSHADERDIR"ShadowFrag.glsl"
  );
  if (!shadowShader->LinkProgram()) return false;

  sceneShader = new Shader(
    MYSHADERDIR"SceneVert.glsl",
    MYSHADERDIR"SceneFrag.glsl"
  );
  if (!sceneShader->LinkProgram()) return false;

  lightShader = new Shader(
    MYSHADERDIR"LightVert.glsl",
    MYSHADERDIR"LightFrag.glsl"
  );
  if (!lightShader->LinkProgram()) return false;

  skyboxShader = new Shader(
    MYSHADERDIR"SkyboxVert.glsl",
    MYSHADERDIR"SkyboxFrag.glsl"
  );
  if (!skyboxShader->LinkProgram()) return false;

  combineShader = new Shader(
    MYSHADERDIR"CombinedVert.glsl",
    MYSHADERDIR"CombinedFrag.glsl"
  );
  if (!combineShader->LinkProgram()) return false;

  bloomShader = new Shader(
    MYSHADERDIR"BloomVert.glsl",
    MYSHADERDIR"BloomFrag.glsl"
  );
  if (!bloomShader->LinkProgram()) return false;

  return true;
}

bool Renderer::InstantiateObjects() {
  quad = Mesh::GenerateQuad();
  root = new SceneNode();
  SceneNode* s;

  // Skybox
  skybox = SOIL_load_OGL_cubemap(
    MYSKYBOXES"nightsky_ft.tga", MYSKYBOXES"nightsky_bk.tga",
    MYSKYBOXES"nightsky_up.tga", MYSKYBOXES"nightsky_dn.tga",
    MYSKYBOXES"nightsky_rt.tga", MYSKYBOXES"nightsky_lf.tga",
    SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0
  );
  if(!skybox) return false;

  // HeightMap
  Mesh* heightMap = new HeightMap(TEXTUREDIR"terrain.raw");
  heightMap->SetTexture(
    SOIL_load_OGL_texture(MYTEXTUREDIR"metal_diffuse.jpg",
    SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS)
  );
  heightMap->SetBumpMap(SOIL_load_OGL_texture(
   MYTEXTUREDIR"metal_normal.jpg", SOIL_LOAD_AUTO,
   SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS
 ));
  if (!heightMap->GetTexture() || !heightMap->GetBumpMap()) return false;
  SetTextureRepeating(heightMap->GetTexture(), true);
  SetTextureRepeating(heightMap->GetBumpMap(), true);

  s = new SceneNode();
  s->SetColour(Vector4(1, 1, 1, 1));
  s->SetTransform(Matrix4::Translation(Vector3(
    RAW_WIDTH * HEIGHTMAP_X / -2, 0, RAW_HEIGHT * HEIGHTMAP_Z / -2
  )));
  s->SetModelScale(Vector3(1, 1, 1));
  s->SetBoundingRadius(1000000.0f);
  s->SetMesh(heightMap);
  root->AddChild(s);

  // Hellknight
  MD5FileData* hellData = new MD5FileData(MESHDIR"hellknight.md5mesh");
  MD5Node* hellNode = new MD5Node(*hellData);

  hellData->AddAnim(MESHDIR"idle2.md5anim");
  hellNode->PlayAnim(MESHDIR"idle2.md5anim");

  hellNode->SetTransform(Matrix4::Translation(Vector3(0, 0, 0)));

  hellNode->SetModelScale(Vector3(0.05, 0.05, 0.05));
  hellNode->SetBoundingRadius(10000.0f);
  root->AddChild(hellNode);

  // Beams
  Mesh* m = Mesh::GenerateQuad();
  m->SetTexture(
    SOIL_load_OGL_texture(MYTEXTUREDIR"white.png",
    SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS)
  );
  SetTextureRepeating(m->GetTexture(), true);

  for(int x = -1; x <= 1; x += 2) {
    for(int z = -1; z <= 1; z += 2) {
      s = new SceneNode();
      int height = 200;
      s->SetTransform(Matrix4::Translation(Vector3(x * 20, height - 10, z * 20)));
      s->SetModelScale(Vector3(1, height, 1));
      s->SetRotation(Vector3(1, 90, 1));
      s->SetBoundingRadius(10000.0f);
      s->SetColour(Vector4(1, 1, 1, 1));
      s->SetMesh(m);
      root->AddChild(s);
    }
  }

  return true;
}

bool Renderer::InstantiateLights() {
  Light* l;

  sphere = new OBJMesh();
  if (!sphere->LoadOBJMesh(MESHDIR"ico.obj")) return false;

  // Moon
  l = new Light();
  l->SetPosition(Vector3(200, 200, 175));
  l->SetColour(Vector4(0.4, 0.6, 1, 1.0f));
  l->SetRadius(2500);
  lights.push_back(l);

  return true;
}
