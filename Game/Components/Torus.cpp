#include "Torus.h"

Torus::Torus(const Texture &texture, int index) : Mesh{texture}, index{index}, light{0.0f} {
}

void Torus::generateMesh(int iSlices, float outer, float inner, float) {
      float a = (outer - inner) / 2;
      float b = inner + a;

      //inner: b - a
      //outer: 2a + b
      //torus: a

      int n = iSlices + 1;
      float s = -360.0f / (float) iSlices;
      glm::vec4 top(a, 0.0f, 0.0f, 1.0f);
      glm::vec4 normal_up(0.0f, 1.0f, 0.0f, 1.0f);
      glm::mat4 I(1.0f);
      glm::vec3 X(1.0f, 0.0f, 0.0f);
      glm::vec3 Y(0.0f, 1.0f, 0.0f);
      glm::vec3 Z(0.0f, 0.0f, 1.0f);
      Vertex temp_vertex;
      for (int i = 0; i < n; i++) {//小圆
            for (int j = 0; j < n; j++) {//大圆
                  glm::mat4 trans;
                  trans = glm::rotate(I, glm::radians(s * i), Y);
                  trans = glm::translate(trans, glm::vec3(b, 0.0f, 0.0f));
                  trans = glm::rotate(trans, glm::radians(s * j), Z);
                  temp_vertex.position = trans * top;

                  trans = glm::rotate(I, glm::radians(s * i), Y);
                  trans = glm::rotate(trans, glm::radians(s * j), Z);
                  temp_vertex.normal = trans * normal_up;

                  temp_vertex.texCoord = glm::vec2(5.0f / (float) iSlices * j * 4, 5.0f / (float) iSlices * i);
                  vertexes.push_back(temp_vertex);
            }
      }
      for (int i = 0; i < iSlices; i++) {
            for (int j = 0; j < iSlices; j++) {
                  indexes.push_back(i * n + j);
                  indexes.push_back((i + 1) * n + j + 1);
                  indexes.push_back((i + 1) * n + j);

                  indexes.push_back(i * n + j);
                  indexes.push_back(i * n + j + 1);
                  indexes.push_back((i + 1) * n + j + 1);
            }
      }
}

void Torus::chosen() {
      light = 0.3f;
}

void Torus::released() {
      light = 0.0f;
}

Torus::Torus(const Torus &torus) = default;

Torus &Torus::operator=(const Torus &torus) {
      if (this == &torus)return *this;
      this->VAO = torus.VAO;
      this->VBO = torus.VBO;
      this->EBO = torus.EBO;
      this->light = torus.light;
      this->index = torus.index;
      this->indexes = torus.indexes;
      this->texture = torus.texture;
      this->vertexes = torus.vertexes;
      this->drawMethod = torus.drawMethod;
      return *this;
}


