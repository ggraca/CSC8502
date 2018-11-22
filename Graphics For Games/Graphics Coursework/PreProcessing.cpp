#include "Renderer.h"

void Renderer::DrawShadowScene() {
  SetCurrentShader(shadowShader);
  glViewport(0, 0, SHADOWSIZE, SHADOWSIZE);
  glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

  int i = 0;
  for(int al : activeLights) {
    glFramebufferTexture2D(
      GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowTex[i], 0
    );
    glClear(GL_DEPTH_BUFFER_BIT);

    projMatrix = (i == 0) ? sunPerspective : lightPerspective;
    viewMatrix = Matrix4::BuildViewMatrix(lights[al]->GetPosition(), Vector3(0, 50, 0));
    shadowMatrix[i] = biasMatrix * (projMatrix * viewMatrix);
    UpdateShaderMatrices();

    // Terrain
    modelMatrix.ToIdentity();
    glUniformMatrix4fv(
      glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"),
      1, false,
      (float*)&modelMatrix
    );
    terrain->Draw();

    // Nodes
    DrawNodes(false);

    i++;
  }

  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  glViewport(0, 0, width, height);
}

void Renderer::DrawTerrain() {
  SetCurrentShader(terrainShader);
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

  glUniform1i(
    glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0
  );
  glUniform1i(
    glGetUniformLocation(currentShader->GetProgram(), "bumpTex"), 1
  );

  int arr[5] = {20, 21, 22, 23, 24};
  glUniform1iv(
    glGetUniformLocation(currentShader->GetProgram(), "terrainTex"),
    5, (int*)&arr
  );

  for(int i = 0; i < 5; i++) {
    glActiveTexture(GL_TEXTURE0 + arr[i]);
    glBindTexture(GL_TEXTURE_2D, terrainTex[i]);
  }

  projMatrix = cameraPerspective;
  viewMatrix = camera->BuildViewMatrix();
  modelMatrix.ToIdentity();
  UpdateShaderMatrices();

  terrain->Draw();
}

void Renderer::DrawObjects() {
  SetCurrentShader(sceneShader);

  glUniform1i(
    glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0
  );
  glUniform1i(
    glGetUniformLocation(currentShader->GetProgram(), "bumpTex"), 1
  );

  projMatrix = cameraPerspective;
  viewMatrix = camera->BuildViewMatrix();
  modelMatrix.ToIdentity();
  UpdateShaderMatrices();

  DrawNodes();
}

void Renderer::DrawGrass() {
  SetCurrentShader(grassShader);

  glUniform1i(
    glGetUniformLocation(currentShader->GetProgram(), "bloom"), bloom
  );

  projMatrix = cameraPerspective;
  viewMatrix = camera->BuildViewMatrix();
  modelMatrix.ToIdentity();
  UpdateShaderMatrices();

  terrain->Draw();
}

void Renderer::DrawWater() {
  SetCurrentShader(waterShader);

  glUniform3fv(
    glGetUniformLocation(currentShader->GetProgram(), "cameraPos"),
    1, (float*)&camera->GetPosition()
  );

  glUniform1i(
    glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0
  );
  glUniform1i(
    glGetUniformLocation(currentShader->GetProgram(), "cubeTex"), 2
  );

  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_CUBE_MAP, currentSkybox);

  projMatrix = cameraPerspective;
  viewMatrix = camera->BuildViewMatrix();
  modelMatrix.ToIdentity();
  UpdateShaderMatrices();

  water->Draw();
}


void Renderer::DrawLights() {
  SetCurrentShader(lightShader);
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  glBlendFunc(GL_ONE, GL_ONE);

  int arr[5] = {10, 11, 12, 13, 14};
  glUniform1i(
    glGetUniformLocation(currentShader->GetProgram(), "depthTex"), 2
  );
  glUniform1i(
    glGetUniformLocation(currentShader->GetProgram(), "normTex"), 3
  );

  glUniform1iv(
    glGetUniformLocation(currentShader->GetProgram(), "shadowTex"),
    MAX_SHADOWS, (int*)&arr
  );
  glUniformMatrix4fv(
    glGetUniformLocation(currentShader->GetProgram(), "shadowMatrix"),
    MAX_SHADOWS, false, (float*)&shadowMatrix
  );
  glUniform1i(
    glGetUniformLocation(currentShader->GetProgram(), "softShadows"),
    softShadows
  );
  glUniform1i(
    glGetUniformLocation(currentShader->GetProgram(), "totalShadows"),
    activeLights.size()
  );

  glUniform3fv(
    glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1,
    (float*)&camera->GetPosition()
  );
  glUniform2f(
    glGetUniformLocation(currentShader->GetProgram(), "pixelSize"),
    1.0f / width, 1.0f / height
  );

  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, objectDepthTex);

  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_2D, objectNormalTex);

  for(int i = 0; i < MAX_SHADOWS; i++) {
    glActiveTexture(GL_TEXTURE0 + arr[i]);
    glBindTexture(GL_TEXTURE_2D, shadowTex[i]);
  }

  for(int i : activeLights) {
    Light* l = lights[i];
    SetShaderLight(*l);

    float radius = l->GetRadius();
    float dist = (l->GetPosition() - camera->GetPosition()).Length();
    if (dist < radius) glCullFace(GL_FRONT);
    else glCullFace(GL_BACK);

    modelMatrix =
      Matrix4::Translation(l->GetPosition()) *
      Matrix4::Scale(Vector3(radius, radius, radius));

    UpdateShaderMatrices();

    sphere->Draw();
  }

  glCullFace(GL_BACK);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


void Renderer::DrawSkybox() {
  SetCurrentShader(skyboxShader);

  glDisable(GL_CULL_FACE);
  glDepthMask(GL_FALSE);

  glUniform1i(
    glGetUniformLocation(currentShader->GetProgram(), "cubeTex"), 4
  );

  glActiveTexture(GL_TEXTURE4);
  glBindTexture(GL_TEXTURE_CUBE_MAP, currentSkybox);

  projMatrix = cameraPerspective;
  UpdateShaderMatrices();

  quad->Draw();

  glDepthMask(GL_TRUE);
  glEnable(GL_CULL_FACE);
}


void Renderer::DrawCombinedScene() {
  SetCurrentShader(combineShader);

  glUniform1i(
    glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 5
  );
  glUniform1i(
    glGetUniformLocation(currentShader->GetProgram(), "depthTex"), 6
  );
  glUniform1i(
    glGetUniformLocation(currentShader->GetProgram(), "emissiveTex"), 7
  );
  glUniform1i(
    glGetUniformLocation(currentShader->GetProgram(), "specularTex"), 8
  );

  glActiveTexture(GL_TEXTURE5);
  glBindTexture(GL_TEXTURE_2D, objectColourTex);

  glActiveTexture(GL_TEXTURE6);
  glBindTexture(GL_TEXTURE_2D, objectDepthTex);

  glActiveTexture(GL_TEXTURE7);
  glBindTexture(GL_TEXTURE_2D, lightEmissiveTex);

  glActiveTexture(GL_TEXTURE8);
  glBindTexture(GL_TEXTURE_2D, lightSpecularTex);

  projMatrix = orthPerspective;
  UpdateShaderMatrices();

  quad->Draw();
}
