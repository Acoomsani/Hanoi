#include "Shader.h"

int Shader::num = 0;

Shader::Shader(const string &vertexPath, const string &fragmentPath) {
      num++;
      string vertexCode;
      string fragmentCode;

      ifstream vShaderFile;
      ifstream fShaderFile;

      vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
      fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);

      try {
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);

            stringstream vShaderStream, fShaderStream;
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();

            vShaderFile.close();
            fShaderFile.close();

            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
      } catch (ifstream::failure &e) {
            std::cerr << "Shader Error: " << e.what() << std::endl;
      }

      //creat shader
      GLuint vertex, fragment;
      vertex = Call(glCreateShader(GL_VERTEX_SHADER));
      fragment = Call(glCreateShader(GL_FRAGMENT_SHADER));

      //source
      const char *vSSource = vertexCode.c_str();
      const char *fSSource = fragmentCode.c_str();
      Call(glShaderSource(vertex, 1, &vSSource, nullptr));
      Call(glShaderSource(fragment, 1, &fSSource, nullptr));

      //compile
      GLint success = 0;
      char info[1024];
      Call(glCompileShader(vertex));
      checkShaderErrors(vertex, "COMPILE");
      Call(glCompileShader(fragment));
      checkShaderErrors(fragment, "COMPILE");

      //creat program
      program = Call(glCreateProgram());

      Call(glAttachShader(program, vertex));
      Call(glAttachShader(program, fragment));

      Call(glLinkProgram(program));
      checkShaderErrors(program, "LINK");

      //delete shader
      Call(glDeleteShader(vertex));
      Call(glDeleteShader(fragment));
}

Shader::~Shader() {
      num--;
      if (num == 0) {
            glDeleteProgram(program);
      }
};

void Shader::begin() const {
      Call(glUseProgram(program));
}

void Shader::end() const {
      Call(glUseProgram(0));
}

GLuint Shader::getProgram() const {
      return program;
}

void Shader::checkShaderErrors(GLuint target, const string &type) const {
      GLint success = 0;
      char info[1024];
      if (type == "COMPILE") {
            Call(glGetShaderiv(target, GL_COMPILE_STATUS, &success));
            if (!success) {
                  Call(glGetShaderInfoLog(program, 1024, nullptr, info));
                  std::cout << info << std::endl;
            }
      } else if (type == "LINK") {
            Call(glGetProgramiv(target, GL_LINK_STATUS, &success));
            if (!success) {
                  Call(glGetShaderInfoLog(program, 1024, nullptr, info));
                  std::cout << info << std::endl;
            }
      } else {
            std::cerr << "ERROR_CHECK ERROR" << std::endl;
      }
}

void Shader::setFloat(const string &name, float value) const {
      GLint location = Call(glGetUniformLocation(program, name.c_str()));
      Call(glUniform1f(location, value));
}

void Shader::setVector3(const string &name, float x, float y, float z) const {
      GLint location = Call(glGetUniformLocation(program, name.c_str()));
      Call(glUniform3f(location, x, y, z));
}

void Shader::setVector3(const string &name, const float *value) const {
      GLint location = Call(glGetUniformLocation(program, name.c_str()));
      Call(glUniform3fv(location, 1, value));
}

void Shader::setInt(const string &name, int value) const {
      GLint location = Call(glGetUniformLocation(program, name.c_str()));
      Call(glUniform1i(location, value));
}

void Shader::setMatrix4x4(const string &name, glm::mat4 value) const {
      GLint location = Call(glGetUniformLocation(program, name.c_str()));
      Call(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value)));
}

Shader::Shader(const Shader &s) : program(s.program) {
      num++;
}
