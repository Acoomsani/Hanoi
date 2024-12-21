#include "Application.h"

bool Application::movable = false;
Application *Application::application = nullptr;

Application::Application() : width(0), height(0) {}

Application *Application::getApplication() {
      if (!application)application = new Application();
      return application;
}

Application::~Application() = default;

bool Application::init(int width_, int height_, const std::string &name) {
      width_ = width_;
      height_ = height_;

      glfwInit();
      glfwWindowHint(GL_SAMPLES, 4);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

      window = glfwCreateWindow(width_, height_, name.c_str(), nullptr, nullptr);
      if (!window)return false;
      glfwMakeContextCurrent(window);

      if (!gladLoadGL()) {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return false;
      }

      Call(glEnable(GL_MULTISAMPLE));
      Call(glEnable(GL_LINE_SMOOTH));
      Call(glLineWidth(10));
      Call(glHint(GL_LINE_SMOOTH_HINT, GL_NICEST));
      Call(glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST));
      Call(glClearColor(0.92, 0.36, 0.1254, 1.0));

      glfwSetWindowUserPointer(window, this);
      glfwSetKeyCallback(window, keyCallBack);
      glfwSetScrollCallback(window, ScrollCallBack);
      glfwSetCursorPosCallback(window, cursorPosCallback);
      glfwSetFramebufferSizeCallback(window, frameBufferSizeCallBack);
      return true;
}

void Application::titleFps() const {
      static double time0 = glfwGetTime();
      static double time1;
      static double dt;
      static int dFrame = -1;
      static std::stringstream info;
      time1 = glfwGetTime();
      dFrame++;
      if ((dt = time1 - time0) >= 1) {
            info.precision(1);
            info << "Tower of Hanoi  " << std::fixed << dFrame / dt << " FPS";
            glfwSetWindowTitle(window, info.str().c_str());
            info.str("");//别忘了在设置完窗口标题后清空所用的string stream
            time0 = time1;
            dFrame = 0;
      }
}

bool Application::update() const {
      if (glfwWindowShouldClose(window)) {
            return false;
      }Call();
      Call(glfwPollEvents());
      Call(glfwSwapBuffers(window));
      return true;
}

void Application::destroy() { glfwTerminate(); }

void Application::setOnResizeCallBack(void(*resizeCallBack)(int, int)) {
      onResizeCallBack = resizeCallBack;
}

void Application::frameBufferSizeCallBack(GLFWwindow *window_, int width_, int height_) {
      auto *app = (Application *) glfwGetWindowUserPointer(window_);
      if (app->onResizeCallBack)
            app->onResizeCallBack(width_, height_);
}

void Application::setOnKeyCallBack(void (*onKeyBoard_)(int, int, int, int)) {
      onKeyCallBack = onKeyBoard_;
}

void Application::keyCallBack(GLFWwindow *window_, int key, int scancode, int action, int mods) {
      auto *app = (Application *) glfwGetWindowUserPointer(window_);
      if (app->onKeyCallBack)
            app->onKeyCallBack(key, scancode, action, mods);
}

void Application::cursorPosCallback(GLFWwindow *window_, double x, double y) {
      auto *app = (Application *) glfwGetWindowUserPointer(window_);
      if (app->onCursorPosCallback)
            app->onCursorPosCallback(x, y, movable);
}

void Application::ScrollCallBack(GLFWwindow *window_, double x, double y) {
      auto *app = (Application *) glfwGetWindowUserPointer(window_);
      if (app->onScrollCallBack)
            app->onScrollCallBack(x, y);
}

void Application::setOnCursorPosCallback(void (*onCursorPosCallBack_)(double, double, bool)) {
      onCursorPosCallback = onCursorPosCallBack_;
}

void Application::setOnScrollCallback(void (*onScrollCallback)(double, double)) {
      onScrollCallBack = onScrollCallback;
}

void Application::setInputMode(int mode, int value) const {
      Call(glfwSetInputMode(window, mode, value));
}

void Application::enable(int function) {
      Call(glEnable(function));
}

void Application::setBlendFunc(int a, int b) {
      Call(glBlendFunc(a, b));
}