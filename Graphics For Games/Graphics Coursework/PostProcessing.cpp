#include "Renderer.h"

void Renderer::DrawBlur() {
  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);
  SetCurrentShader(bloomShader);
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

  glUniform2f(
    glGetUniformLocation(currentShader->GetProgram(), "pixelSize"),
    1.0f / width, 1.0f / height
  );
  glUniform1i(
    glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 9
  );

  glActiveTexture(GL_TEXTURE9);
  glBindTexture(GL_TEXTURE_2D, combinedColourTex);

  projMatrix = orthPerspective;
  viewMatrix.ToIdentity();
  UpdateShaderMatrices();

  quad->Draw();
  glUseProgram(0);
  glEnable(GL_CULL_FACE);
}

void Renderer::DrawBloom() {
  glDisable(GL_CULL_FACE);
  SetCurrentShader(bloomShader);
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

  glUniform1i(
    glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 9
  );

  glActiveTexture(GL_TEXTURE9);
  glBindTexture(GL_TEXTURE_2D, combinedColourTex);

  projMatrix = orthPerspective;
  viewMatrix.ToIdentity();
  UpdateShaderMatrices();

  quad->Draw();
  glUseProgram(0);
  glEnable(GL_CULL_FACE);
}
