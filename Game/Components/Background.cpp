#include <stb_image.h>
#include "Background.h"

BackGround::BackGround(int unit) : unit(unit) {

}

void BackGround::loadCubeMap(std::vector<std::string> faces) {
      glGenTextures(1, &textureId);
      glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
      int width, height, nrChannels;
      for (unsigned int i = 0; i < faces.size(); i++) {
            unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
            if (data) {
                  glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                               0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
                  );
                  stbi_image_free(data);
            } else {
                  std::cout << "Cubemap texture failed to load at path: " << faces[i] <<
                            std::endl;
                  stbi_image_free(data);
            }
      }
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void BackGround::generateMesh() {
      float skyboxVertices_[108] = {
              // positions
              -1.0f, 1.0f, -1.0f,
              -1.0f, -1.0f, -1.0f,
              1.0f, -1.0f, -1.0f,
              1.0f, -1.0f, -1.0f,
              1.0f, 1.0f, -1.0f,
              -1.0f, 1.0f, -1.0f,

              -1.0f, -1.0f, 1.0f,
              -1.0f, -1.0f, -1.0f,
              -1.0f, 1.0f, -1.0f,
              -1.0f, 1.0f, -1.0f,
              -1.0f, 1.0f, 1.0f,
              -1.0f, -1.0f, 1.0f,

              1.0f, -1.0f, -1.0f,
              1.0f, -1.0f, 1.0f,
              1.0f, 1.0f, 1.0f,
              1.0f, 1.0f, 1.0f,
              1.0f, 1.0f, -1.0f,
              1.0f, -1.0f, -1.0f,

              -1.0f, -1.0f, 1.0f,
              -1.0f, 1.0f, 1.0f,
              1.0f, 1.0f, 1.0f,
              1.0f, 1.0f, 1.0f,
              1.0f, -1.0f, 1.0f,
              -1.0f, -1.0f, 1.0f,

              -1.0f, 1.0f, -1.0f,
              1.0f, 1.0f, -1.0f,
              1.0f, 1.0f, 1.0f,
              1.0f, 1.0f, 1.0f,
              -1.0f, 1.0f, 1.0f,
              -1.0f, 1.0f, -1.0f,

              -1.0f, -1.0f, -1.0f,
              -1.0f, -1.0f, 1.0f,
              1.0f, -1.0f, -1.0f,
              1.0f, -1.0f, -1.0f,
              -1.0f, -1.0f, 1.0f,
              1.0f, -1.0f, 1.0f
      };
      for (int i = 0; i < 108; ++i) {
            skyboxVertices[i] = skyboxVertices_[i];
      }
      glGenVertexArrays(1, &skyboxVAO);
      glGenBuffers(1, &skyboxVBO);
      glBindVertexArray(skyboxVAO);
      glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
}

void BackGround::render(const Shader &skyboxShader, const Camera &camera) const {
      // draw skybox as last
      glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
      skyboxShader.begin();
      skyboxShader.setInt("skybox", 0);
      glm::mat4 view = glm::mat4(glm::mat3(camera.getViewMatrix())); // remove translation from the view matrix
      skyboxShader.setMatrix4x4("view", view);
      glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.34f, 0.1f, 100.0f);
      skyboxShader.setMatrix4x4("projection", projection);
      // skybox cube
      glBindVertexArray(skyboxVAO);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
      glDrawArrays(GL_TRIANGLES, 0, 36);
      glBindVertexArray(0);
      glDepthFunc(GL_LESS); // set depth function back to default
}

void BackGround::render(const Shader &skyboxShader, Camera &camera, int) const {
      // draw skybox as last
      glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
      skyboxShader.begin();
      skyboxShader.setInt("skybox", 0);
      static double start = glfwGetTime();
      double end = glfwGetTime();
      camera.mouseCall(8.0f * (float) (end - start), 0.0f, true);
      start = end;
      glm::mat4 view = glm::mat4(glm::mat3(camera.getViewMatrix())); // remove translation from the view matrix
      skyboxShader.setMatrix4x4("view", view);
      glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.34f, 0.1f, 100.0f);
      skyboxShader.setMatrix4x4("projection", projection);
      // skybox cube
      glBindVertexArray(skyboxVAO);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
      glDrawArrays(GL_TRIANGLES, 0, 36);
      glBindVertexArray(0);
      glDepthFunc(GL_LESS); // set depth function back to default
}