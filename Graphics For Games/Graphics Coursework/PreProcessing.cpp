#include "Renderer.h"

void Renderer::SetupShadowFBO() {
  glGenTextures(1, &shadowTex);
  glBindTexture(GL_TEXTURE_2D, shadowTex);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  glTexImage2D(
    GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOWSIZE,
    SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL
  );

  glTexParameteri(
    GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE
  );
  glBindTexture(GL_TEXTURE_2D, 0);

  glGenFramebuffers(1, &shadowFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
  glFramebufferTexture2D(
    GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowTex, 0
  );

  glDrawBuffer(GL_NONE);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::DrawShadowScene() {
  glClear(GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, SHADOWSIZE, SHADOWSIZE);
  glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

  SetCurrentShader(shadowShader);

  projMatrix = lightPerspective;
  viewMatrix = Matrix4::BuildViewMatrix(lights[0]->GetPosition(), Vector3(0, 0, 0));
  shadowMatrix = biasMatrix * (lightPerspective * viewMatrix);
  UpdateShaderMatrices();
  DrawNodes();

  glUseProgram(0);
  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  glViewport(0, 0, width, height);
}
