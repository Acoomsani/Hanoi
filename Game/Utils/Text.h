// @brief: 字体渲染类

#include "General.h"
#include <Utils/TextUtil.h>
#include <Render/GlFramework/Shader.h>

#pragma once

class Text {
public:
    Text(const Text &) = delete;

    Text(const std::map<GLchar, Character> &characters);

    void renderText(const Shader &shader, const std::string &text, glm::vec2 position, GLfloat scale, glm::vec3 color) const;

    void renderText(const Shader& shader, const std::wstring& text, glm::vec2 position, GLfloat scale, glm::vec3 color) const;
private:
    GLuint VAO{}, VBO{};
    const std::map<GLchar, Character> characters{};
};