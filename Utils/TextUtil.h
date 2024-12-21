// @brief: 字符加载工具类
// @birth: created on 2024-12-16

#include "General.h"
#include <Render/Wrapper/CheckError.h>

#pragma once

struct Character {
    GLuint textureId;
    glm::ivec2 size;
    glm::ivec2 Bearing;
    GLuint Advance;
};

class TextUtil {
public:

    //加载字符
    void loadCharacters(GLuint *unit, const std::string &font);

    //用于加载汉语字符，未使用
    void loadCharacters(GLuint* unit, const std::string& font,const std::wstring &text);

private:
    using map = std::map<GLchar, Character>;
    FT_Face face{};
    FT_Library ft{};
    GS(map, characters);
};