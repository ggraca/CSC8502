#include "Renderer.h"

bool Renderer::BuildScenes() {
  camera = new Camera(-20, 270, Vector3(-80, 90, 0));
  SceneNode* s;

  // Skybox
  skybox[0] = SOIL_load_OGL_cubemap(
    MYSKYBOXES"nightsky_ft.tga", MYSKYBOXES"nightsky_bk.tga",
    MYSKYBOXES"nightsky_up.tga", MYSKYBOXES"nightsky_dn.tga",
    MYSKYBOXES"nightsky_rt.tga", MYSKYBOXES"nightsky_lf.tga",
    SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0
  );
  if(!skybox[0]) return false;

  // Skybox
  skybox[1] = SOIL_load_OGL_cubemap(
    MYSKYBOXES"sorbin_ft.tga", MYSKYBOXES"sorbin_bk.tga",
    MYSKYBOXES"sorbin_up.tga", MYSKYBOXES"sorbin_dn.tga",
    MYSKYBOXES"sorbin_rt.tga", MYSKYBOXES"sorbin_lf.tga",
    SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0
  );
  if(!skybox[1]) return false;

  // HeightMap
  terrain = new HeightMap(TEXTUREDIR"terrain.raw");
  LoadTexture(terrain, "sand_diffuse.jpg");
  LoadBumpMap(terrain, "sand_normal.jpg");

  string names[5] = {
    "mountain_diffuse.jpg",
    "mountain_diffuse.jpg",
    "sand_diffuse.jpg",
    "grass_diffuse.jpg",
    "rock2_diffuse.jpg"
  };
  for(int i = 0; i < 5; i++) {
    terrainTex[i] = SOIL_load_OGL_texture(
      (string(MYTEXTUREDIR) + names[i]).c_str(), SOIL_LOAD_AUTO,
      SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS
    );
    SetTextureRepeating(terrainTex[i], true);
    glBindTexture(GL_TEXTURE_2D, terrainTex[i]);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
  }

  // Water
  water = new HeightMap(TEXTUREDIR"terrain.raw", true);
  LoadTexture(water, "water_diffuse.jpg");
  LoadBumpMap(water, "water_normal.jpg");

  Light* l;
  sphere = new OBJMesh();
  if (!sphere->LoadOBJMesh(MESHDIR"ico.obj")) return false;

  // Moon
  l = new Light();
  l->SetRadius(4000);
  lights.push_back(l);

  // Beams
  for(int x = -1; x <= 1; x += 2) {
    for(int z = -1; z <= 1; z += 2) {
      l = new Light();
      l->SetPosition(Vector3(x * 20, 90, z * 20));
      l->SetColour(Vector4(0.9f, 0.9f, 0.9f, 0.0f));
      l->SetRadius(50);
      lights.push_back(l);
    }
  }

  // Hellknight
  MD5FileData* hellData = new MD5FileData(MESHDIR"hellknight.md5mesh");
  MD5Node* hellNode = new MD5Node(*hellData);

  hellData->AddAnim(MESHDIR"idle2.md5anim");
  hellNode->PlayAnim(MESHDIR"idle2.md5anim");
  hellNode->SetModelScale(Vector3(0.1, 0.1, 0.1));
  hellNode->SetTransform(Matrix4::Translation(Vector3(10, 59, 0)));
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
      int height = 400;
      SceneNode* col = new SceneNode();
      col->SetTransform(Matrix4::Translation(Vector3(x * 20, height - 20, z * 20)));
      root->AddChild(col);

      s = new SceneNode();
      s->SetColour(Vector4(1, 1, 1, 0));
      s->SetMesh(m);
      s->SetTransform(Matrix4::Translation(Vector3(0, 0, -1)));
      s->SetRotation(Vector3(1, 0, 1));
      s->SetModelScale(Vector3(1.1, height, 1.1));
      s->SetBoundingRadius(10000.0f);
      col->AddChild(s);

      s = new SceneNode();
      s->SetColour(Vector4(1, 1, 1, 0));
      s->SetMesh(m);
      s->SetTransform(Matrix4::Translation(Vector3(-1, 0, 0)));
      s->SetRotation(Vector3(1, 90, 1));
      s->SetModelScale(Vector3(1.1, height, 1.1));
      s->SetBoundingRadius(10000.0f);
      col->AddChild(s);

      s = new SceneNode();
      s->SetColour(Vector4(1, 1, 1, 0));
      s->SetMesh(m);
      s->SetTransform(Matrix4::Translation(Vector3(0, 0, 1)));
      s->SetRotation(Vector3(1, 180, 1));
      s->SetModelScale(Vector3(1.1, height, 1.1));
      s->SetBoundingRadius(10000.0f);
      col->AddChild(s);

      s = new SceneNode();
      s->SetColour(Vector4(1, 1, 1, 0));
      s->SetMesh(m);
      s->SetTransform(Matrix4::Translation(Vector3(1, 0, 0)));
      s->SetRotation(Vector3(1, 270, 1));
      s->SetModelScale(Vector3(1.1, height, 1.1));
      s->SetBoundingRadius(10000.0f);
      col->AddChild(s);
    }
  }

  SelectSceneA();

  return true;
}

void Renderer::SelectSceneA() {
  activeLights.clear();
  activeLights.push_back(0);
  activeLights.push_back(1);
  activeLights.push_back(2);
  activeLights.push_back(3);
  activeLights.push_back(4);
  softShadows = true;

  lights[0]->SetColour(Vector4(0.4, 0.6, 1, 1.0f));
  lights[0]->SetPosition(Vector3(1000, 750, 800));
  currentSkybox = skybox[0];
  rotateLight = false;

  bloom = true;

  camera->SetPosition(Vector3(-80, 90, 0));
}

void Renderer::SelectSceneB() {
  activeLights.clear();
  activeLights.push_back(0);
  softShadows = false;

  lights[0]->SetColour(Vector4(1, 0.6, 0.3, 1.0f));
  lights[0]->SetPosition(Vector3(-1000, 750, -800));
  currentSkybox = skybox[1];
  //rotateLight = true;

  bloom = false;

  camera->SetPosition(Vector3(-50, 90, -280));
}
