#include "Renderer.h"

void Renderer::DrawBlur(GLuint colourTex) {
  SetCurrentShader(blurShader);
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

  glUniform2f(glGetUniformLocation(
    currentShader->GetProgram(), "pixelSize"), 1.0f / width, 1.0f / height
  );

  bool isVertical = false;
  quad->SetTexture(colourTex);
  for(int i = 0; i < POST_PASSES; i++) {
    glFramebufferTexture2D(
      GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
      blurColourTex[!isVertical], 0
    );
    glUniform1i(glGetUniformLocation(
      currentShader->GetProgram(), "isVertical"), isVertical
    );
    quad->Draw();

    quad->SetTexture(blurColourTex[!isVertical]);
    isVertical = !isVertical;
  }
}

void Renderer::DrawBloom(GLuint colourTex) {
  // Filter
  SetCurrentShader(bloomFilterShader);
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

  quad->SetTexture(colourTex);
  quad->Draw();

  // Blur
  glBindFramebuffer(GL_FRAMEBUFFER, blurFBO);
  DrawBlur(bloomColourTex);

  // Combine
  glBindFramebuffer(GL_FRAMEBUFFER, bloomFBO);
  SetCurrentShader(bloomCombineShader);
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

  glUniform1i(
    glGetUniformLocation(currentShader->GetProgram(), "bloomTex"), 9
  );

  glActiveTexture(GL_TEXTURE9);
  glBindTexture(GL_TEXTURE_2D, blurColourTex[0]);

  quad->SetTexture(colourTex);
  quad->Draw();
}

void Renderer::DrawUnderwater(GLuint colourTex) {
  SetCurrentShader(underwaterShader);
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

  glUniform2f(glGetUniformLocation(
    currentShader->GetProgram(), "pixelSize"), 1.0f / width, 1.0f / height
  );

  glUniform1f(
    glGetUniformLocation(currentShader->GetProgram(), "cameraHeight"),
    camera->GetPosition().y
  );

  quad->SetTexture(colourTex);
  quad->Draw();
}

void Renderer::PresentScene(GLuint colourTex) {
  SetCurrentShader(basicShader);
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

  quad->SetTexture(colourTex);
  quad->Draw();
}
