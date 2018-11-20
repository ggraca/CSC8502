#include "Renderer.h"

bool Renderer::SetupShaders() {

  basicShader = new Shader(
    MYSHADERDIR"BasicVert.glsl",
    MYSHADERDIR"BasicFrag.glsl"
  );
  if (!basicShader->LinkProgram()) return false;

  shadowShader = new Shader(
    MYSHADERDIR"ShadowVert.glsl",
    MYSHADERDIR"ShadowFrag.glsl"
  );
  if (!shadowShader->LinkProgram()) return false;

  sceneShader = new Shader(
    MYSHADERDIR"SceneVert.glsl",
    MYSHADERDIR"SceneFrag.glsl"
  );
  if (!sceneShader->LinkProgram()) return false;

  lightShader = new Shader(
    MYSHADERDIR"LightVert.glsl",
    MYSHADERDIR"LightFrag.glsl"
  );
  if (!lightShader->LinkProgram()) return false;

  skyboxShader = new Shader(
    MYSHADERDIR"SkyboxVert.glsl",
    MYSHADERDIR"SkyboxFrag.glsl"
  );
  if (!skyboxShader->LinkProgram()) return false;

  combineShader = new Shader(
    MYSHADERDIR"CombinedVert.glsl",
    MYSHADERDIR"CombinedFrag.glsl"
  );
  if (!combineShader->LinkProgram()) return false;

  bloomFilterShader = new Shader(
    MYSHADERDIR"BasicVert.glsl",
    MYSHADERDIR"BloomFilterFrag.glsl"
  );
  if (!bloomFilterShader->LinkProgram()) return false;

  bloomCombineShader = new Shader(
    MYSHADERDIR"BasicVert.glsl",
    MYSHADERDIR"BloomCombineFrag.glsl"
  );
  if (!bloomCombineShader->LinkProgram()) return false;

  blurShader = new Shader(
    MYSHADERDIR"BasicVert.glsl",
    MYSHADERDIR"BlurFrag.glsl"
  );
  if (!blurShader->LinkProgram()) return false;

  return true;
}