// @brief: opengl纹理相关函数封装类

#include "General.h"
#include <Render/Wrapper/CheckError.h>

#pragma once

using std::string;

class Texture {
public:
    Texture(const Texture &texture);

    Texture&operator=(const Texture&);

    Texture(const string &path, unsigned int unit);

    virtual ~Texture();

    void bind() const;

private:
    int *num{};//引用计数
    int width{0};
    int height{0};
    GLuint texture{0};
    GS(unsigned int, unit);
};

