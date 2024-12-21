// @brief: 渲染所需顶点坐标及VAO、VBO、EBO抽象类

#include "General.h"
#include "Render/GlFramework/Shader.h"
#include "Render/GlFramework/Texture.h"

#pragma once

struct Vertex {
    glm::vec4 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
};

class Mesh {
public:
    Mesh(const Mesh&mesh);
    Mesh(const Texture &texture);

    void setup();

    Texture& get_texture() {
          return texture;
    }

protected:
    int drawMethod{};
    Texture texture;
    std::vector<GLuint> indexes;//顶点索引
    std::vector<Vertex> vertexes;//顶点坐标

    GLuint VAO{}, VBO{}, EBO{};
public:

    //生成网格
    virtual void generateMesh(int, float, float, float) = 0;

    //渲染模式 三角形/线条模式
    void setDrawMethod(int method) {
          drawMethod = method;
    }

    //渲染
    virtual void render(const Shader &shader);
};