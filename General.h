#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <utility>
#include <future>
#include <vector>
#include <queue>
#include <stack>
#include <iomanip>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <span>
#include <memory>
#include <functional>
#include <concepts>
#include <algorithm>
#include <format>
#include <chrono>
#include <numeric>
#include <numbers>
#include <random>

//GLM
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
//如果你惯用左手坐标系，在此定义GLM_FORCE_LEFT_HANDED
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

//stb_image.h
#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#define GS(type, name)type name; public: [[nodiscard]] type get_##name() const {return name;} void set_##name(const type & name_) {name = name_;}

namespace status {
    enum class Status {
        released = 0, pressed, floating
    };

    struct OptionState {
        const static glm::vec3 &getLight(Status status) {
              switch (status) {
                    case Status::released:
                          return Released;
                    case Status::pressed:
                          return Pressed;
                    case Status::floating:
                          return Floating;
              }
        }

        static constexpr glm::vec3 Released{0.0f};
        static constexpr glm::vec3 Pressed{-0.3f, -0.3f, -0.3f};
        static constexpr glm::vec3 Floating{0.3f, 0.3f, 0.3f};
    };
};