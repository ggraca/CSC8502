#include "Renderer.h"

bool Renderer::SetupFBOs() {
  glGenFramebuffers(1, &shadowFBO);
  glGenFramebuffers(1, &objectFBO);
  glGenFramebuffers(1, &lightFBO);
  glGenFramebuffers(1, &combinedFBO);

  glGenTextures(1, &shadowTex);

  // Shadows
  if (!SetupShadowFBO()) return false;

  // Deferred Rendering
  if (!SetupObjectFBO()) return false;
  if (!SetupLightFBO()) return false;
  if (!SetupCombinedFBO()) return false;

  return true;
}


bool Renderer::SetupShadowFBO() {
  // Textures
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

  // Association
  glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
  glFramebufferTexture2D(
    GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowTex, 0
  );

  glDrawBuffer(GL_NONE);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  return true;
}


bool Renderer::SetupObjectFBO() {
  GLenum buffers[2];
  buffers[0] = GL_COLOR_ATTACHMENT0;
  buffers[1] = GL_COLOR_ATTACHMENT1;

  // Generate our scene depth texture
  GenerateScreenTexture(objectDepthTex, true);
  GenerateScreenTexture(objectColourTex);
  GenerateScreenTexture(objectNormalTex);

  // Association
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

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  return true;
}


bool Renderer::SetupLightFBO() {
  GLenum buffers[2];
  buffers[0] = GL_COLOR_ATTACHMENT0;
  buffers[1] = GL_COLOR_ATTACHMENT1;

  // Textures
  GenerateScreenTexture(lightEmissiveTex);
  GenerateScreenTexture(lightSpecularTex);

  // Association
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

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  return true;
}


bool Renderer::SetupCombinedFBO() {
  GLenum buffers[1];
  buffers[0] = GL_COLOR_ATTACHMENT0;

  // Textures
  GenerateScreenTexture(combinedColourTex);

  // Association
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