#include "Pillar.h"

Pillar::Pillar(const Texture &texture) : Mesh{texture}, light{0.0f} {

}

void Pillar::generateMesh(int iSlices, float height, float radius, float) {
      int n = iSlices + 1;
      float s = -360.0f / (float) iSlices;
      glm::vec4 top(radius, 0.0f, 0.0f, 1.0f);
      glm::vec4 normal_up(0.0f, 1.0f, 0.0f, 1.0f);
      glm::mat4 I(1.0f);
      glm::vec3 X(1.0f, 0.0f, 0.0f);
      glm::vec3 Y(0.0f, 1.0f, 0.0f);
      glm::vec3 Z(0.0f, 0.0f, 1.0f);
      Vertex temp_vertex;
      for (int i = 0; i < n; i++) {
            glm::mat4 trans;
            trans = glm::rotate(I, glm::radians(s * i), Y);
            temp_vertex.position = trans * top;
            trans = glm::rotate(I, glm::radians(s * i), Y);
            temp_vertex.normal = trans * normal_up;
            temp_vertex.texCoord = glm::vec2(10.0f / (float) iSlices * i, 0);
            vertexes.push_back(temp_vertex);
            trans = glm::translate(trans, glm::vec3(0.0f, height, 0.0f));
            temp_vertex.position = trans * top;
            temp_vertex.texCoord = glm::vec2(10.0f / (float) iSlices * i, 20);
            vertexes.push_back(temp_vertex);
      }
      for (int i = 0; i < iSlices * 2; i++) {
            indexes.push_back(i);
            indexes.push_back(i + 1);
            indexes.push_back(i + 2);
      }
}

void Pillar::chosen() {
      light = 0.3f;
}

void Pillar::released() {
      light = 0.0f;
}
