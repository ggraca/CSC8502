#include "Renderer.h"

void Renderer::DrawShadowScene() {
  SetCurrentShader(shadowShader);
  glClear(GL_DEPTH_BUFFER_BIT);

  glViewport(0, 0, SHADOWSIZE, SHADOWSIZE);
  glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

  projMatrix = lightPerspective;
  viewMatrix = Matrix4::BuildViewMatrix(lights[0]->GetPosition(), Vector3(0, 0, 0));
  shadowMatrix = biasMatrix * (lightPerspective * viewMatrix);
  UpdateShaderMatrices();
  DrawNodes();

  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  glViewport(0, 0, width, height);
}


void Renderer::DrawObjects() {
  SetCurrentShader(sceneShader);
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

  glUniform1i(
    glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0
  );
  glUniform1i(
    glGetUniformLocation(currentShader->GetProgram(), "bumpTex"), 1
  );

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, objectDepthTex);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, objectNormalTex);

  projMatrix = cameraPerspective;
  viewMatrix = camera->BuildViewMatrix();
  modelMatrix.ToIdentity();
  UpdateShaderMatrices();

  DrawNodes();
}


void Renderer::DrawLights() {
  SetCurrentShader(lightShader);
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  glBlendFunc(GL_ONE, GL_ONE);

  glUniform1i(
    glGetUniformLocation(currentShader->GetProgram(), "depthTex"), 2
  );
  glUniform1i(
    glGetUniformLocation(currentShader->GetProgram(), "normTex"), 3
  );
  glUniform1i(
    glGetUniformLocation(currentShader->GetProgram(), "shadowTex"), 4
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

  glActiveTexture(GL_TEXTURE4);
  glBindTexture(GL_TEXTURE_2D, shadowTex);

  for(auto l : lights) {
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

  projMatrix = cameraPerspective;
  UpdateShaderMatrices();

  quad->Draw();

  glDepthMask(GL_TRUE);
  glEnable(GL_CULL_FACE);
}


void Renderer::CombineBuffers() {
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
