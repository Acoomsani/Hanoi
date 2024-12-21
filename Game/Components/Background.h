// @brief: 渲染背景相关函数封装类

#include "General.h"
#include <Render/Application/Camera.h>
#include <Render/GlFramework/Shader.h>
#include <Render/GlFramework/Texture.h>

#pragma once

class BackGround {
public:
    BackGround(int unit);

    void render(const Shader &skyboxShader, Camera &camera,int) const;

    void render(const Shader &skyboxShader, const Camera &camera) const;

    void generateMesh();

    void loadCubeMap(std::vector<std::string> faces);

private:
    int unit;
    GLuint textureId{};
    float skyboxVertices[108]{};
    GLuint skyboxVAO{}, skyboxVBO{};
};