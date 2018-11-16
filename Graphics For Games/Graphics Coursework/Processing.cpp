#include "Renderer.h"

bool Renderer::SetupDeferredRenderingFBO() {
  glGenFramebuffers(1, &objectFBO);
  glGenFramebuffers(1, &lightFBO);
  glGenFramebuffers(1, &combinedFBO);

  GLenum buffers[2];
  buffers[0] = GL_COLOR_ATTACHMENT0;
  buffers[1] = GL_COLOR_ATTACHMENT1;

  // Generate our scene depth texture
  GenerateScreenTexture(objectDepthTex, true);
  GenerateScreenTexture(objectColourTex);
  GenerateScreenTexture(objectNormalTex);
  GenerateScreenTexture(lightEmissiveTex);
  GenerateScreenTexture(lightSpecularTex);
  GenerateScreenTexture(combinedColourTex);

  // Attach them to our object FBO
  glBindFramebuffer(GL_FRAMEBUFFER, objectFBO);
  glFramebufferTexture2D(
    GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, objectColourTex, 0
  );
  glFramebufferTexture2D(
    GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, objectNormalTex, 0
  );
  glFramebufferTexture2D(
    GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, objectDepthTex, 0
  );
  glDrawBuffers(2, buffers);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    return false;

  // Attach them to our lightning FBO
  glBindFramebuffer(GL_FRAMEBUFFER, lightFBO);
  glFramebufferTexture2D(
    GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, lightEmissiveTex, 0
  );
  glFramebufferTexture2D(
    GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, lightSpecularTex, 0
  );
  glDrawBuffers(2, buffers);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    return false;

  // Attach them to our combined FBO
  glBindFramebuffer(GL_FRAMEBUFFER, combinedFBO);
  glFramebufferTexture2D(
    GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, combinedColourTex, 0
  );
  glDrawBuffers(1, buffers);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    return false;

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  return true;
}

void Renderer::DrawObjects() {
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

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

  glUseProgram(0);
}

void Renderer::DrawLights() {
  SetCurrentShader(lightShader);
  glBindFramebuffer(GL_FRAMEBUFFER, lightFBO);

  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  glBlendFunc(GL_ONE, GL_ONE);

  glUniform1i(
    glGetUniformLocation(currentShader->GetProgram(), "depthTex"), 3
  );
  glUniform1i(
    glGetUniformLocation(currentShader->GetProgram(), "normTex"), 4
  );
  glUniform1i(
    glGetUniformLocation(currentShader->GetProgram(), "shadowTex"), 5
  );
  glUniform3fv(
    glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1,
    (float*)&camera->GetPosition()
  );
  glUniform2f(
    glGetUniformLocation(currentShader->GetProgram(), "pixelSize"),
    1.0f / width, 1.0f / height
  );

  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_2D, objectDepthTex);

  glActiveTexture(GL_TEXTURE4);
  glBindTexture(GL_TEXTURE_2D, objectNormalTex);

  glActiveTexture(GL_TEXTURE5);
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

  glClearColor(0.2f, 0.2f, 0.2f, 1);
  glUseProgram(0);
}

void Renderer::DrawSkybox() {
  glDisable(GL_CULL_FACE);
  glDepthMask(GL_FALSE);
  SetCurrentShader(skyboxShader);

  projMatrix = cameraPerspective;
  UpdateShaderMatrices();

  quad->Draw();

  glUseProgram(0);
  glDepthMask(GL_TRUE);
  glEnable(GL_CULL_FACE);
}

void Renderer::CombineBuffers() {
  SetCurrentShader(combineShader);

  glUniform1i(
    glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 2
  );
  glUniform1i(
    glGetUniformLocation(currentShader->GetProgram(), "depthTex"), 3
  );
  glUniform1i(
    glGetUniformLocation(currentShader->GetProgram(), "emissiveTex"), 4
  );
  glUniform1i(
    glGetUniformLocation(currentShader->GetProgram(), "specularTex"), 5
  );

  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, objectColourTex);

  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_2D, objectDepthTex);

  glActiveTexture(GL_TEXTURE4);
  glBindTexture(GL_TEXTURE_2D, lightEmissiveTex);

  glActiveTexture(GL_TEXTURE5);
  glBindTexture(GL_TEXTURE_2D, lightSpecularTex);

  projMatrix = orthPerspective;
  UpdateShaderMatrices();

  quad->Draw();
  glUseProgram(0);
}
