// @brief: table

#include "General.h"
#include "Mesh.h"

#pragma once

class Table : public Mesh {
public:
    Table(const Texture &texture);

    Table(Table &&) = default;

    void generateMesh(int unused, float length, float width, float height);
};

