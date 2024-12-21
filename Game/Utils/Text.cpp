#include "Text.h"

Text::Text(const std::map<GLchar, Character> &characters) : characters(characters) {
      glGenVertexArrays(1, &VAO);
      glGenBuffers(1, &VBO);
      glBindVertexArray(VAO);
      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindVertexArray(0);
}

void Text::renderText(const Shader &shader, const std::string &text, glm::vec2 position, GLfloat scale, glm::vec3 color) const {
      float x = position.x;
      float y = position.y;
      // 激活对应的渲染状态
      shader.begin();
      shader.setVector3("textColor", color.x, color.y, color.z);
      glActiveTexture(GL_TEXTURE0);
      glBindVertexArray(VAO);
      // 遍历文本中所有的字符
      std::string::const_iterator c;
      for (c = text.begin(); c != text.end(); c++) {
            Character ch = characters.at(*c);

            GLfloat xPos = x + (float) ch.Bearing.x * scale;
            GLfloat yPos = y - (float) (ch.size.y - ch.Bearing.y) * scale;

            GLfloat w = (float) ch.size.x * scale;
            GLfloat h = (float) ch.size.y * scale;
            // 对每个字符更新VBO
            GLfloat vertices[6][4] = {
                    {xPos,     yPos + h, 0.0, 0.0},
                    {xPos,     yPos,     0.0, 1.0},
                    {xPos + w, yPos,     1.0, 1.0},

                    {xPos,     yPos + h, 0.0, 0.0},
                    {xPos + w, yPos,     1.0, 1.0},
                    {xPos + w, yPos + h, 1.0, 0.0}
            };
            // 在四边形上绘制字形纹理
            glBindTexture(GL_TEXTURE_2D, ch.textureId);
            // 更新VBO内存的内容
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            // 绘制四边形
            glDrawArrays(GL_TRIANGLES, 0, 6);
            // 更新位置到下一个字形的原点，注意单位是1/64像素
            x += (float) (ch.Advance >> 6) * scale; // 位偏移6个单位来获取单位为像素的值 (2^6 = 64)
      }
      glBindVertexArray(0);
      glBindTexture(GL_TEXTURE_2D, 0);
}

void Text::renderText(const Shader& shader, const std::wstring& text, glm::vec2 position, GLfloat scale, glm::vec3 color) const {
    float x = position.x;
    float y = position.y;
    // 激活对应的渲染状态
    shader.begin();
    shader.setVector3("textColor", color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);
    // 遍历文本中所有的字符
    std::wstring::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) {
        Character ch = characters.at(*c);

        GLfloat xPos = x + (float)ch.Bearing.x * scale;
        GLfloat yPos = y - (float)(ch.size.y - ch.Bearing.y) * scale;

        GLfloat w = (float)ch.size.x * scale;
        GLfloat h = (float)ch.size.y * scale;
        // 对每个字符更新VBO
        GLfloat vertices[6][4] = {
                {xPos,     yPos + h, 0.0, 0.0},
                {xPos,     yPos,     0.0, 1.0},
                {xPos + w, yPos,     1.0, 1.0},

                {xPos,     yPos + h, 0.0, 0.0},
                {xPos + w, yPos,     1.0, 1.0},
                {xPos + w, yPos + h, 1.0, 0.0}
        };
        // 在四边形上绘制字形纹理
        glBindTexture(GL_TEXTURE_2D, ch.textureId);
        // 更新VBO内存的内容
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // 绘制四边形
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // 更新位置到下一个字形的原点，注意单位是1/64像素
        x += (float)(ch.Advance >> 6) * scale; // 位偏移6个单位来获取单位为像素的值 (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}