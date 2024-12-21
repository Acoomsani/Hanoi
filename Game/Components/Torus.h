// @brief: 圆盘

#include "Mesh.h"

#pragma once

class Torus : public Mesh {
public:

    Torus(const Torus&torus);

    Torus&operator=(const Torus&torus);

    Torus(const Texture &texture, int index);

    void generateMesh(int iSlices, float outer, float inner,float unused);

    void chosen();

    void released();

private:
    GS(int, index);
    GS(float, light);
};