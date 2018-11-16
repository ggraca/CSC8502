#pragma once

#include <string>
#include <iostream>
#include <fstream>

#include "Mesh.h"

#define RAW_WIDTH 257
#define RAW_HEIGHT 257

#define HEIGHTMAP_X 1.0f
#define HEIGHTMAP_Z 1.0f
#define HEIGHTMAP_Y 0.11f
#define HEIGHTMAP_TEX_X 1.0f / 4.0f
#define HEIGHTMAP_TEX_Z 1.0f / 4.0f

class HeightMap : public Mesh {
public:
  HeightMap(string name);
  ~HeightMap(void) {}
};
