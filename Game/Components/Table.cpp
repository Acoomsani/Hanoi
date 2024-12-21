#include "Table.h"

Table::Table(const Texture &texture) : Mesh(texture) {

}

void Table::generateMesh(int, float length, float width, float height) {
      float len = length / 2;
      float len_t = length * 1.5f;
      float wid = width / 2;
      float wid_t = width * 1.5f;
      Vertex temp_vertex{};

      temp_vertex = {{-len,   0.0f, -wid, 1.0f},
                     {0.0f,   1.0f, 0.0f},
                     {-len_t, -wid_t}};
      vertexes.push_back(temp_vertex);
      temp_vertex = {{-len,   0.0f, wid, 1.0f},
                     {0.0f,   1.0f, 0.0f},
                     {-len_t, wid_t}};
      vertexes.push_back(temp_vertex);
      temp_vertex = {{len,   0.0f, wid, 1.0f},
                     {0.0f,  1.0f, 0.0f},
                     {len_t, wid_t}};
      vertexes.push_back(temp_vertex);
      temp_vertex = {{len,   0.0f, -wid, 1.0f},
                     {0.0f,  1.0f, 0.0f},
                     {len_t, -wid_t}};
      vertexes.push_back(temp_vertex);

      temp_vertex = {{-len,   -height, -wid, 1.0f},
                     {0.0f,   1.0f,    0.0f},
                     {-len_t, -wid_t}};
      vertexes.push_back(temp_vertex);
      temp_vertex = {{-len,   -height, wid, 1.0f},
                     {0.0f,   1.0f,    0.0f},
                     {-len_t, wid_t}};
      vertexes.push_back(temp_vertex);
      temp_vertex = {{len,   -height, wid, 1.0f},
                     {0.0f,  1.0f,    0.0f},
                     {len_t, wid_t}};
      vertexes.push_back(temp_vertex);
      temp_vertex = {{len,   -height, -wid, 1.0f},
                     {0.0f,  1.0f,    0.0f},
                     {len_t, -wid_t}};
      vertexes.push_back(temp_vertex);

      indexes = {
              0, 1, 2,
              0, 2, 3,
              0, 4, 1,
              1, 4, 5,
              1, 5, 6,
              2, 1, 6,
              2, 6, 7,
              3, 2, 7,
      };
}