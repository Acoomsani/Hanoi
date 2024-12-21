#include "Mesh.h"

Mesh::Mesh(const Texture &texture) : texture(texture) {}


Mesh::Mesh(const Mesh &mesh) = default;


void Mesh::setup() {
      Call(glCreateVertexArrays(1, &VAO));
      Call(glBindVertexArray(VAO));
      Call(glCreateBuffers(1, &VBO));
      Call(glBindBuffer(GL_ARRAY_BUFFER, VBO));
      Call(glNamedBufferStorage(VBO, sizeof(Vertex) * vertexes.size(), &vertexes[0], 0));
      Call(glGenBuffers(1, &EBO));
      Call(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
      Call(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * indexes.size(), &indexes[0], GL_STATIC_DRAW));
      Call(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) 0));
      Call(glEnableVertexAttribArray(0));
      Call(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, normal)));
      Call(glEnableVertexAttribArray(1));
      Call(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, texCoord)));
      Call(glEnableVertexAttribArray(2));
      Call(glBindVertexArray(0));
}

void Mesh::render(const Shader &shader) {
      shader.begin();
      texture.bind();
      Call(glBindVertexArray(VAO));
      switch (drawMethod) {
            case 0:
            Call(glDrawElements(GL_TRIANGLES, indexes.size(), GL_UNSIGNED_INT, 0));
                  break;
            case 1:
            Call(glDrawElements(GL_LINE_STRIP, indexes.size(), GL_UNSIGNED_INT, 0));
                  break;
      }
}
