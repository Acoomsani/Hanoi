// @brief: 菜单类

#include "Option.h"
#include "General.h"
#include "Message.h"
#include "Game/Utils/Text.h"
#include <Render/GlFramework/Shader.h>

#pragma once

class Menu {
public:
    Menu(int width, int height);

    std::unordered_map<std::string, Option> &get_options() {
          return options;
    }

    void release();

    void renderTitle(const Shader &textShader, const Text &text);

    void renderOptions(const Shader &textShader, const Text &text);

    void renderInput(const Shader &textShader, const Text &text, int val);

    void renderContext(const Shader &contextShader, int b_height,const std::vector<Message>&messages) const;

private:
    int width,height;
    std::unordered_map<std::string, Option> texts;
    std::unordered_map<std::string, Option> options;
};