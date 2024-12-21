// @brief: opengl着色器相关函数封装类

#include "General.h"
#include "Render/Wrapper/CheckError.h"

#pragma once

using std::string;
using std::stringstream;
using std::ifstream;

class Shader {
public:
    Shader(const string &vertexPath, const string &fragmentPath);

    Shader(const Shader &);

    ~Shader();

    [[nodiscard]] GLuint getProgram() const;

    void begin() const;

    void end() const;

    void setFloat(const string &name, float value) const;

    void setVector3(const string &name, float x, float y, float z) const;

    void setVector3(const string &name, const float *value) const;

    void setInt(const string &name, int value) const;

    void setMatrix4x4(const string &name, glm::mat4 value) const;

private:
    void checkShaderErrors(GLuint target, const string &type) const;

private:
    static int num;
    GLuint program{0};
};
