#include "../../nclgl/window.h"
#include "Renderer.h"
#include <time.h>

#pragma comment(lib, "nclgl.lib")

int main() {
  Window w("Next UE4!", 1920, 1200, false);
  if (!w.HasInitialised()) {
    return -1;
  }

  srand(time(NULL));

  Renderer renderer(w);
  if (!renderer.HasInitialised()) {
    return -1;
  }

  w.LockMouseToWindow(true);
  w.ShowOSPointer(false);

  while (w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)) {
    renderer.UpdateScene(w.GetTimer()->GetTimedMS());
    renderer.RenderScene();
  }
  return 0;
}
