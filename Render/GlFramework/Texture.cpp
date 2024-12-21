#include "Texture.h"

#include <stb_image.h>

#include <memory>

Texture::Texture(const string &path, unsigned int unit) {
      num = new int{0};
      (*num)++;
      this->unit = unit;
      int channels;

      stbi_set_flip_vertically_on_load(true);

      unsigned char *data = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);

      Call(glGenTextures(1, &texture));
      Call(glBindTexture(GL_TEXTURE_2D, texture));
      Call(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
      Call(glGenerateMipmap(GL_TEXTURE_2D));

      stbi_image_free(data);

      Call(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
      Call(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR));

      Call(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
      Call(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
}

Texture::~Texture() {
      (*num)--;
      if ((*num) == 0 && texture != 0) {
            //Call(glDeleteTextures(1, &texture));
            delete num;
      }
}

void Texture::bind() const {
      Call(glActiveTexture(GL_TEXTURE0 + unit));
      Call(glBindTexture(GL_TEXTURE_2D, texture));
}

Texture::Texture(const Texture &texture) : texture(texture.texture), width(texture.width), height(texture.height), unit(texture.unit),
                                           num(texture.num) {
      (*texture.num)++;
}

Texture &Texture::operator=(const Texture &texture_) {
      if (&texture_ == this)return *this;
      (*num)--;
      (*texture_.num)++;
      this->num = texture_.num;
      this->unit = texture_.unit;
      this->width = texture_.width;
      this->height = texture_.height;
      this->texture = texture_.texture;
      return *this;
}
