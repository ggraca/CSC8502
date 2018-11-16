#include "Renderer.h"

void Renderer::DrawBloom() {
  SetCurrentShader(bloomShader);
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

  glUniform1i(
    glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0
  );

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, combinedColourTex);

  projMatrix = orthPerspective;
  viewMatrix.ToIdentity();
  UpdateShaderMatrices();

  quad->Draw();

  glUseProgram(0);
}
