#include <Game/Utils/Text.h>
#include "General.h"

#pragma once

struct Message {
    float scale1;//缩放
    float scale2;
    Text &text1;//字体渲染类
    Text &text2;
    int offset1;//偏移量
    int offset2;//偏移量
    int b_width;//起始宽度
    glm::vec3 color1;
    glm::vec3 color2;
    glm::vec3 light{};//用于渲染是否被选中
    std::string message1;
    std::string message2;
};
