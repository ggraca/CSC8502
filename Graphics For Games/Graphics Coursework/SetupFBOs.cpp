#include "Renderer.h"

bool Renderer::SetupFBOs() {
  glGenFramebuffers(1, &shadowFBO);
  glGenFramebuffers(1, &objectFBO);
  glGenFramebuffers(1, &lightFBO);
  glGenFramebuffers(1, &combinedFBO);
  glGenFramebuffers(1, &blurFBO);
  glGenFramebuffers(1, &bloomFBO);
  glGenFramebuffers(1, &underwaterFBO);

  // Shadows
  if (!SetupShadowFBO()) return false;

  // Deferred Rendering
  if (!SetupObjectFBO()) return false;
  if (!SetupLightFBO()) return false;
  if (!SetupCombinedFBO()) return false;
  if (!SetupBlurFBO()) return false;
  if (!SetupBloomFBO()) return false;
  if (!SetupUnderwaterFBO()) return false;
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  return true;
}


bool Renderer::SetupShadowFBO() {
  // Textures
  for(int i = 0; i < MAX_SHADOWS; i++)
    GenerateShadowTexture(shadowTex[i]);

  // Association
  glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
  glFramebufferTexture2D(
    GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowTex[0], 0
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

  return true;
}

bool Renderer::SetupBlurFBO() {
  GenerateScreenTexture(blurColourTex[0]);
  GenerateScreenTexture(blurColourTex[1]);

  return true;
}

bool Renderer::SetupBloomFBO() {
  GLenum buffers[1];
  buffers[0] = GL_COLOR_ATTACHMENT0;

  // Textures
  GenerateScreenTexture(bloomColourTex);

  // Association
  glBindFramebuffer(GL_FRAMEBUFFER, bloomFBO);
  glFramebufferTexture2D(
    GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bloomColourTex, 0
  );

  glDrawBuffers(1, buffers);
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    return false;

  return true;
}

bool Renderer::SetupUnderwaterFBO() {
  GLenum buffers[1];
  buffers[0] = GL_COLOR_ATTACHMENT0;

  // Textures
  GenerateScreenTexture(underwaterColourTex);

  // Association
  glBindFramebuffer(GL_FRAMEBUFFER, underwaterFBO);
  glFramebufferTexture2D(
    GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, underwaterColourTex, 0
  );

  glDrawBuffers(1, buffers);
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    return false;

  return true;
}
