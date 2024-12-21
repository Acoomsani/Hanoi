// @brief: opengl、glfw相关函数封装类

#include "General.h"
#include "Render/wrapper/CheckError.h"

#pragma once

class Application {
private:
    Application();

    ~Application();

    static Application *application;

    GS(int, width);
    GS(int, height);

    static bool movable;

    GLFWwindow *window{nullptr};

    void (*onResizeCallBack)(int, int){nullptr};

    void (*onKeyCallBack)(int, int, int, int){nullptr};

    void (*onScrollCallBack)(double, double){nullptr};

    void (*onCursorPosCallback)(double, double, bool){nullptr};

    static void ScrollCallBack(GLFWwindow *, double, double);

    static void frameBufferSizeCallBack(GLFWwindow *, int, int);

    static void keyCallBack(GLFWwindow *, int, int, int, int);

    static void cursorPosCallback(GLFWwindow *, double, double);

public:
    Application(const Application &) = delete;

    Application &operator=(const Application &) = delete;

    bool init(int width_ = 800, int height_ = 600, const std::string &name = "window");

    static Application *getApplication();

    bool update() const;

    static void destroy();

    void titleFps() const;

    static void enable(int function);

    static void setBlendFunc(int, int);

    static bool getMovable() { return movable; }

    static void setMovable(bool mo) { movable = mo; }

    void setOnResizeCallBack(void(*)(int, int));

    void setInputMode(int mode, int value) const;

    void setOnKeyCallBack(void(*)(int, int, int, int));

    void setOnCursorPosCallback(void(*)(double, double, bool));

    void setOnScrollCallback(void(*)(double xOff, double yOff));

    [[nodiscard]] GLFWwindow *getWindow() const {
          if (application)return window;
          else {
                application = new Application;
                return window;
          }
    }
};