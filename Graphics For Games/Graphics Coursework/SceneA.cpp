#include "Renderer.h"

bool Renderer::BuildSceneA() {
  camera = new Camera(-20, 270, Vector3(-80, 20, 0));
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

  hellNode->SetModelScale(Vector3(0.1, 0.1, 0.1));
  hellNode->SetTransform(Matrix4::Translation(Vector3(10, 0, 0)));
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
      int height = 250;
      SceneNode* col = new SceneNode();
      col->SetTransform(Matrix4::Translation(Vector3(x * 20, height - 10, z * 20)));
      root->AddChild(col);

      s = new SceneNode();
      s->SetColour(Vector4(1, 1, 1, 0));
      s->SetMesh(m);
      s->SetTransform(Matrix4::Translation(Vector3(0, 0, -1)));
      s->SetRotation(Vector3(1, 0, 1));
      s->SetModelScale(Vector3(1, height, 1));
      s->SetBoundingRadius(10000.0f);
      col->AddChild(s);

      s = new SceneNode();
      s->SetColour(Vector4(1, 1, 1, 0));
      s->SetMesh(m);
      s->SetTransform(Matrix4::Translation(Vector3(-1, 0, 0)));
      s->SetRotation(Vector3(1, 90, 1));
      s->SetModelScale(Vector3(1, height, 1));
      s->SetBoundingRadius(10000.0f);
      col->AddChild(s);

      s = new SceneNode();
      s->SetColour(Vector4(1, 1, 1, 0));
      s->SetMesh(m);
      s->SetTransform(Matrix4::Translation(Vector3(0, 0, 1)));
      s->SetRotation(Vector3(1, 180, 1));
      s->SetModelScale(Vector3(1, height, 1));
      s->SetBoundingRadius(10000.0f);
      col->AddChild(s);

      s = new SceneNode();
      s->SetColour(Vector4(1, 1, 1, 0));
      s->SetMesh(m);
      s->SetTransform(Matrix4::Translation(Vector3(1, 0, 0)));
      s->SetRotation(Vector3(1, 270, 1));
      s->SetModelScale(Vector3(1, height, 1));
      s->SetBoundingRadius(10000.0f);
      col->AddChild(s);

    }
  }

  Light* l;

  sphere = new OBJMesh();
  if (!sphere->LoadOBJMesh(MESHDIR"ico.obj")) return false;

  // Moon
  l = new Light();
  l->SetPosition(Vector3(200, 150, 175));
  l->SetColour(Vector4(0.4, 0.6, 1, 1.0f));
  l->SetRadius(500);
  lights.push_back(l);

  // Beams
  for(int x = -1; x <= 1; x += 2) {
    for(int z = -1; z <= 1; z += 2) {
      l = new Light();
      l->SetPosition(Vector3(x * 20, 30, z * 20));
      l->SetColour(Vector4(0.6f, 0.6f, 0.6f, 1.0f));
      l->SetRadius(50);
      lights.push_back(l);
    }
  }

  return true;
}
