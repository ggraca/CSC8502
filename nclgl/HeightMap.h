#pragma once

#include <string>
#include <iostream>
#include <fstream>

#include "Mesh.h"

#define RAW_WIDTH 513
#define RAW_HEIGHT 513

#define HEIGHTMAP_X 3.0f
#define HEIGHTMAP_Z 3.0f
#define HEIGHTMAP_Y 1.5f
#define HEIGHTMAP_TEX_X 1.0f / 2.0f
#define HEIGHTMAP_TEX_Z 1.0f / 2.0f

class HeightMap : public Mesh {
public:
  HeightMap(string name, bool flat = false);
  ~HeightMap(void) {}
};
