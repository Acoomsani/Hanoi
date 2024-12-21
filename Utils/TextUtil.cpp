#include "TextUtil.h"


void TextUtil::loadCharacters(GLuint *unit, const std::string &font) {
      FT_Init_FreeType(&ft);
      FT_New_Face(ft, font.c_str(), 0, &face);
      FT_Set_Pixel_Sizes(face, 0, 50);
      Call(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
      characters.clear();
      for (int c = 0; c < 128; c++) {
            // 加载字符的字形
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                  std::cout << "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
                  continue;
            }
            (*unit)++;
            Call(glGenTextures(1, unit));
            Call(glBindTexture(GL_TEXTURE_2D, *unit));
            Call(glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
                              face->glyph->bitmap.width,
                              face->glyph->bitmap.rows,
                              0, GL_RED, GL_UNSIGNED_BYTE,
                              face->glyph->bitmap.buffer
            ));
            // 设置纹理选项
            Call(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
            Call(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
            Call(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            Call(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
            // 储存字符供之后使用
            Character character = {
                    *unit,
                    glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                    glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                    static_cast<GLuint>(face->glyph->advance.x)
            };
            characters.insert(std::pair<GLchar, Character>(c, character));
      }
      FT_Done_Face(face);
      FT_Done_FreeType(ft);
}

void TextUtil::loadCharacters(GLuint* unit, const std::string& font,const std::wstring&text) {
    FT_Init_FreeType(&ft);
    FT_New_Face(ft, font.c_str(), 0, &face);
    FT_Set_Pixel_Sizes(face, 0, 50);
    Call(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
    characters.clear();
    for(auto &c : text){
        // 加载字符的字形
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cout << "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
            continue;
        }
        (*unit)++;
        Call(glGenTextures(1, unit));
        Call(glBindTexture(GL_TEXTURE_2D, *unit));
        Call(glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0, GL_RED, GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        ));
        // 设置纹理选项
        Call(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        Call(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        Call(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        Call(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        // 储存字符供之后使用
        Character character = {
                *unit,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<GLuint>(face->glyph->advance.x)
        };
        characters.insert(std::pair<GLchar, Character>(c, character));
    }
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

