#include "General.h"

#pragma once

struct Option {
    float scale;//缩放
    glm::vec3 color;
    glm::vec2 position;
    status::Status status;//状态，选中/悬浮/·
    std::string context;//文本
};