#include "Game.h"

int Game::step{0};//步数
int Game::level{0};//圆盘数
bool Game::got{true};//是否已获得提示（获取提示时是否已计算完，用于开始显示计时）
int Game::width{1400};
int Game::height{900};
std::string Game::s{};//提示字符串（作参数传给另一线程）
float Game::fov{45.0f};//视野
float Game::deltaTime{0.0f};//相机相关
float Game::lastFrame{0.0f};//相机相关
bool Game::firstMouse{true};//相机相关
std::future<int> Game::paths{};//unused
Menu Game::menu{width, height};//菜单
float Game::pillar_height{2.0f};//柱高
unsigned int Game::textureUnit{0};//纹理单元计数
std::vector<Torus> Game::torusM{};//
std::vector<Torus> Game::torusR{};//
std::vector<Torus> Game::torusL{};//左圆盘
Game::Process Game::process{menus};//流程从菜单开始
unsigned int Game::characterUnit{300};//字符纹理单元从300开始
GameRecorder Game::gameRecorder{"record.dat"};//记录文件
float Game::lastX{static_cast<float>(width / 2.0)};
float Game::lastY{static_cast<float>(height / 2.0)};
std::chrono::system_clock::time_point Game::startTime{};//用于计时
std::unordered_map<std::string, Option> Game::options{};//按钮
std::function<void(const Shader &, const Text &)> Game::renderPrompt{};//渲染提示std::function
std::function<void(const Shader &, const Text &)> Game::renderWarning{};//渲染警告std::function

std::unique_ptr<Camera> Game::camera{std::make_unique<Camera>(Camera())};//相机

void Game::menuProcessCursor(const Application &application) {
      using ss = status::Status;
      auto &op = menu.get_options();
      auto height_ = static_cast<float>(height);
      double xPos, yPos, heightB = (float) height - op.at("ngm").position.y, heightT = (float) height - op.at("qut").position.y;
      glfwGetCursorPos(application.window, &xPos, &yPos);
      if (yPos < heightT + 5 && yPos > heightB - 60) {
            if (yPos < height_ + 5 - op.at("ngm").position.y && yPos > height_ - 60 - op.at("ngm").position.y) {
                  menu.release();
                  if (glfwGetMouseButton(app.window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
                        op.at("ngm").status = ss::pressed;
                        level = 0;
                        if (renderMenuLevelInput())//是否退回菜单 true -> 否
                              process = ngm;
                        if (level == -1)level = 0;
                  } else {
                        op.at("ngm").status = ss::floating;
                  }
            } else if (yPos < height_ + 5 - op.at("ogm").position.y && yPos > height_ - 60 - op.at("ogm").position.y) {
                  menu.release();
                  if (glfwGetMouseButton(app.window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
                        op.at("ogm").status = ss::pressed;
                        process = ogm;
                  } else {
                        op.at("ogm").status = ss::floating;
                  }
            } else if (yPos < height_ + 5 - op.at("rec").position.y && yPos > height_ - 60 - op.at("rec").position.y) {
                  menu.release();
                  if (glfwGetMouseButton(app.window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
                        op.at("rec").status = ss::pressed;
                        process = rec;
                  } else {
                        op.at("rec").status = ss::floating;
                  }
            } else if (yPos < height_ + 5 - op.at("des").position.y && yPos > height_ - 60 - op.at("des").position.y) {
                  menu.release();
                  if (glfwGetMouseButton(app.window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
                        op.at("des").status = ss::pressed;
                        process = des;
                  } else {
                        op.at("des").status = ss::floating;
                  }
            } else if (yPos < height_ + 5 - op.at("qut").position.y && yPos > height_ - 60 - op.at("qut").position.y) {
                  menu.release();
                  if (glfwGetMouseButton(app.window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
                        op.at("qut").status = ss::pressed;
                        process = qut;
                  } else {
                        op.at("qut").status = ss::floating;
                  }
            } else if (yPos < height_ + 5 - op.at("demo").position.y && yPos > height_ - 60 - op.at("demo").position.y) {
                  menu.release();
                  if (glfwGetMouseButton(app.window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
                        op.at("demo").status = ss::pressed;
                        process = demo;
                  } else {
                        op.at("demo").status = ss::floating;
                  }
            } else {
                  menu.release();
            }
      } else {
            menu.release();
      }
}

void Game::levelInputKeyCallBack(int key, int scancode, int action, int mods) {
      if (key >= GLFW_KEY_0 && key <= GLFW_KEY_9 && action == GLFW_PRESS) {
            level = level * 10 + key - GLFW_KEY_0;
            if (level >= 100)level = 99;
      } else if (key == GLFW_KEY_ENTER) {
            process = ngm;
      } else if (key == GLFW_KEY_BACKSPACE) {
            level /= 10;
      } else if (key == GLFW_KEY_ESCAPE) {
            level = -1;
            process = menus;
      }
}

void Game::inGameCursorCallBack(double xPosIn, double yPosIn, bool movable) {
      auto xPos = static_cast<float>(xPosIn);
      auto yPos = static_cast<float>(yPosIn);
      if (firstMouse) {
            lastX = xPos;
            lastY = yPos;
            firstMouse = false;
      }
      auto xOffset = xPos - lastX;
      auto yOffset = lastY - yPos;
      lastX = xPos;
      lastY = yPos;
      if (movable)camera->mouseCall(xOffset, yOffset, true);
}

void Game::inGameProcessCursor(const Application &application) {
      auto &op = options;
      using ss = status::Status;
      double xPos, yPos, heightP = (float) height - op.at("A").position.y, heightM = (float) height - op.at("MA").position.y;
      glfwGetCursorPos(application.window, &xPos, &yPos);
      if (yPos < heightP + 10 && yPos > heightP - 55 && xPos > op.at("A").position.x - 15 && xPos < op.at("C").position.x + 55) {
            pillarChoices(xPos);
      } else if (yPos < heightM + 10 && yPos > heightM - 55 && xPos > op.at("MA").position.x - 15 && xPos < op.at("MC").position.x + 210) {
            torusMoveChoices(xPos);
      } else {
            releaseOptions();
      }
}

void Game::releaseTorus() {
      if (!torusM.empty())torusM[0].released();
      if (!torusL.empty())torusL[0].released();
      if (!torusR.empty())torusR[0].released();
}

void Game::releaseOptions() {
      using ss = status::Status;
      options.at("A").status = ss::released;
      options.at("B").status = ss::released;
      options.at("C").status = ss::released;
      options.at("MA").status = ss::released;
      options.at("MB").status = ss::released;
      options.at("MC").status = ss::released;
}

void Game::pillarChoices(double xPos) {
      auto &op = options;
      using ss = status::Status;
      releaseOptions();
      if (xPos < op.at("A").position.x + 55) {
            if (glfwGetMouseButton(app.window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
                  releaseTorus();
                  chosen = &torusL;
                  if (!torusL.empty())torusL[0].chosen();
                  options.at("A").status = ss::pressed;
            } else { options.at("A").status = ss::floating; }
      } else if (xPos < op.at("B").position.x + 55 && xPos > op.at("B").position.x - 15) {
            if (glfwGetMouseButton(app.window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
                  releaseTorus();
                  chosen = &torusM;
                  if (!torusM.empty())torusM[0].chosen();
                  options.at("B").status = ss::pressed;
            } else { options.at("B").status = ss::floating; }
      } else if (xPos > op.at("C").position.x - 15) {
            if (glfwGetMouseButton(app.window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
                  releaseTorus();
                  chosen = &torusR;
                  if (!torusR.empty())torusR[0].chosen();
                  options.at("C").status = ss::pressed;
            } else { options.at("C").status = ss::floating; }
      }
}

void Game::torusMoveChoices(double xPos) {
      auto &op = options;
      using ss = status::Status;
      releaseOptions();
      if (xPos < op.at("MA").position.x + 210) {
            if (glfwGetMouseButton(app.window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
                  releaseTorus();
                  options.at("MA").status = ss::pressed;
                  move(chosen, torusL);
            } else { options.at("MA").status = ss::floating; }
      } else if (xPos < op.at("MB").position.x + 210 && xPos > op.at("MB").position.x - 15) {
            if (glfwGetMouseButton(app.window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
                  releaseTorus();
                  options.at("MB").status = ss::pressed;
                  move(chosen, torusM);
            } else { options.at("MB").status = ss::floating; }
      } else if (xPos > op.at("MC").position.x - 15) {
            if (glfwGetMouseButton(app.window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
                  releaseTorus();
                  options.at("MC").status = ss::pressed;
                  move(chosen, torusR);
            } else { options.at("MC").status = ss::floating; }
      }
}

void Game::inGameKeyCallBack(int key, int scancode, int action, int mods) {
      static bool released = true;
      if (key == GLFW_KEY_ESCAPE)process = menus;
      if (key == GLFW_KEY_P && action == GLFW_PRESS) {
            using namespace std::chrono;
            long long ex = duration_cast<seconds>((system_clock::now() - startTime)).count();
            std::vector<int> L;
            std::vector<int> M;
            std::vector<int> R;
            for (const auto &item: torusL)L.push_back(item.get_index());
            for (const auto &item: torusM)M.push_back(item.get_index());
            for (const auto &item: torusR)R.push_back(item.get_index());
            if (released) {
                  auto now = std::chrono::system_clock::now();
                  std::time_t now_c = std::chrono::system_clock::to_time_t(now);
                  struct tm now_tm = *std::localtime(&now_c);
                  std::ostringstream os;
                  os << std::put_time(&now_tm, "%Y-%m-%d-%H:%M:%S");
                  if (gameRecorder.addRecord({step, level, false, ex, os.str(), {L, M, R}}))
                        renderWarning = [](const Shader &textShader, const Text &text) {
                            string s = "Saved successfully";
                            text.renderText(textShader, s, {width / 2 - s.length() * 48 / 2, height / 2 + 20}, 2, {0.8f, 0.1f, 0.1f});
                        };
                  else
                        renderWarning = [](const Shader &textShader, const Text &text) {
                            string s = "Fail to save";
                            text.renderText(textShader, s, {width / 2 - s.length() * 48 / 2, height / 2 + 20}, 2, {0.8f, 0.1f, 0.1f});
                        };
            }
            released = false;
      }
      if (key == GLFW_KEY_P && action == GLFW_RELEASE) {
            released = true;
      }
      if (key == GLFW_KEY_H && action == GLFW_RELEASE) {
            std::vector<int> L;
            std::vector<int> M;
            std::vector<int> R;
            for (const auto &item: torusL)L.push_back(item.get_index());
            for (const auto &item: torusM)M.push_back(item.get_index());
            for (const auto &item: torusR)R.push_back(item.get_index());
            s = "";
            got = false;
            paths = std::async(getHelp, std::ref(s), &got, L, M, R);
            renderPrompt = [](const Shader &textShader, const Text &text) {
                string s1 = "Calculating";
                if (s.length() <= 0) {
                      text.renderText(textShader, s1, {width / 2 - s1.length() * 36 / 2, height / 2 + 20}, 1.4, {0.8f, 0.1f, 0.1f});
                } else if (s.length() < 13) {
                      text.renderText(textShader, s, {width / 2 - s.length() * 36 / 2, height / 2 + 20}, 1.4, {0.8f, 0.1f, 0.1f});
                } else {
                      text.renderText(textShader, s.substr(0, 12), {width / 2 - 13 * 36 / 2, height / 2 + 20}, 1.4, {0.8f, 0.1f, 0.1f});
                }
            };
      }
      if (key == GLFW_KEY_C && action == GLFW_PRESS) {
            renderPrompt = nullptr;
      }
}

void Game::inGameProcessKeyInput(const Application &application) {
      GLFWwindow *window = application.getWindow();
      auto currentFrame = (float) glfwGetTime();
      deltaTime = currentFrame - lastFrame;
      lastFrame = currentFrame;
      if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
            application.setInputMode(GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            Application::setMovable(true);
      }
      if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
            application.setInputMode(GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            Application::setMovable(false);
      }
      if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera->keyInput(FORWARD, deltaTime);
      if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera->keyInput(BACKWARD, deltaTime);
      if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera->keyInput(LEFT, deltaTime);
      if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera->keyInput(RIGHT, deltaTime);
      if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            camera->keyInput(UP, deltaTime);
      if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            camera->keyInput(DOWN, deltaTime);
}

Game::Game() {
      app.init(width, height,"Tower of Hanoi");
      Application::enable(GL_BLEND);
      Application::enable(GL_DEPTH_TEST);
      app.setOnResizeCallBack([](int width_, int height_) {
          width = width_;
          height = height_;
          firstMouse = true;
          refreshOptionPositions();
          glViewport(0, 0, width_, height_);
      });
      Application::setBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      prepareMatrix();//在prepareShaders前执行
      prepareShaders();
      loadCharacters();
      prepareBackground();
}

Game::~Game() {
      Application::destroy();
}

void Game::prepareMatrix() {
      model = glm::mat4{1.0f};
      view = camera->getViewMatrix();
      projection = glm::perspective(glm::radians(fov), 1.34f, zNear, zFar);
}

void Game::loadCharacters() {
      textUtil.loadCharacters(&characterUnit, "Assets/Fonts/ALGER.ttf");
      textAlger = std::make_shared<Text>(textUtil.get_characters());
      textUtil.loadCharacters(&characterUnit, "Assets/Fonts/COOPBL.ttf");
      textGorte = std::make_shared<Text>(textUtil.get_characters());
      textUtil.loadCharacters(&characterUnit, "Assets/Fonts/FTLTLT.ttf");
      textArlrd = std::make_shared<Text>(textUtil.get_characters());
}

void Game::prepareShaders() {
      Shader inGameShader{"Assets/Shaders/inGame.vert", "Assets/Shaders/inGame.frag"};
      inGameShader.begin();
      inGameShader.setMatrix4x4("viewMatrix", view);
      inGameShader.setMatrix4x4("projection", projection);
      inGameShader.end();
      Shader textShader{"Assets/Shaders/text.vert", "Assets/Shaders/text.frag"};
      textShader.begin();
      textShader.setMatrix4x4("projection", glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height)));
      textShader.end();
      Shader contextShader{"Assets/Shaders/context.vert", "Assets/Shaders/context.frag"};
      contextShader.begin();
      contextShader.setMatrix4x4("projection", glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height)));
      contextShader.end();
      Shader skyboxShader{"Assets/Shaders/skybox.vert", "Assets/Shaders/skybox.frag"};
      skyboxShader.begin();
      skyboxShader.end();
      shaders.insert({"text", textShader});
      shaders.insert({"inGame", inGameShader});
      shaders.insert({"context", contextShader});
      shaders.insert({"skybox", skyboxShader});
}

void Game::prepareBackground() {
      menuBack = std::make_shared<BackGround>(++textureUnit);
      std::string path = "Assets/Images/skybox1/";
      menuBack->loadCubeMap(
              {path + "right.jpg", path + "left.jpg", path + "top.jpg", path + "down.jpg", path + "back.jpg", path + "front.jpg"});
      menuBack->generateMesh();
      path = "Assets/Images/skybox2/";
      inGameBack = std::make_shared<BackGround>(++textureUnit);
      inGameBack->loadCubeMap(
              {path + "right.jpg", path + "left.jpg", path + "top.jpg", path + "down.jpg", path + "back.jpg", path + "front.jpg"});
      inGameBack->generateMesh();
}

void Game::start() {
      while (process) {
            switch (process) {
                  case menus:
                        menuProcess();
                        break;
                  case ngm:
                        startNewGame(level);
                        break;
                  case ogm:
                        oldGameProcess();
                        break;
                  case des:
                        descriptionProcess();
                        break;
                  case rec:
                        recordsProcess();
                        break;
                  case demo:
                        presentation();
                        break;
            }
      }

}

void Game::menuProcess() {
      app.setOnKeyCallBack(nullptr);
      Shader &textShader = shaders.at("text");
      Shader &skyboxShader = shaders.at("skybox");
      textShader.begin();
      while (process == menus) {
            if (!app.update())process = qut;
            Call(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
            skyboxShader.begin();
            menuBack->render(skyboxShader, *camera, 1);
            skyboxShader.end();
            textShader.begin();
            textShader.setMatrix4x4("projection", glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height)));
            menuProcessCursor(app);
            menu.renderTitle(textShader, *textAlger);
            menu.renderOptions(textShader, *textGorte);
            textShader.end();

      }
}

bool Game::renderMenuLevelInput() {
      app.setOnKeyCallBack(levelInputKeyCallBack);
      Shader &textShader = shaders.at("text");
      Shader &skyboxShader = shaders.at("skybox");
      while (process) {
            if (process == ngm) {
                  app.setOnKeyCallBack(nullptr);
                  return true;
            }
            if (level == -1) {
                  app.setOnKeyCallBack(nullptr);
                  return false;
            }
            if (!app.update())process = qut;
            Call(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
            textShader.begin();
            menu.renderInput(textShader, *textArlrd, level);
            textShader.end();
      }
      app.setOnKeyCallBack(nullptr);
      return true;
}

void Game::descriptionProcess() {
      app.setOnKeyCallBack([](int key, int scancode, int action, int mods) {
          if (key == GLFW_KEY_ESCAPE)process = menus;
      });
      app.setOnCursorPosCallback(nullptr);
      std::string aim = "Move all the Disks from the Blue Tower to the Red .";
      std::string method1 = "Press the   A, B or C   button to choose disk which";
      std::string method2 = "you want to move .";
      std::string method3 = "Press the three   MOVE TO X   buttons to move the";
      std::string method4 = "chosen disk to the Tower   X .";
      std::string move1 = "Change the View by using the   W, A, S or D   keys";
      std::string move2 = "Or by holding down the right mouse button .";
      std::string other = "Press  H  to get Prompt and press  P  to save game .";
      std::vector<Message> messages{
              //title
              {1.4, 0.0, *textGorte, *textAlger, 0,  0,  width / 2 - 11 * 40 / 2, {0.0f, 0.0f, 1.0f}, {},                 {}, "Description", ""},
              //aim{},
              {1.2, 0.9, *textAlger, *textArlrd, 70, 50, 140,                     {0.1f, 0.9f, 0.1f}, {0.1f, 0.1f, 0.7f}, {}, "Aim",         aim},
              //method{},
              {1.2, 0.0, *textAlger, *textArlrd, 80, 0,  140,                     {0.1f, 0.9f, 0.1f}, {},                 {}, "Playing Method",},
              {0.9, 0.9, *textArlrd, *textArlrd, 50, 50, 140,                     {0.1f, 0.1f, 0.7f}, {0.1f, 0.1f, 0.7f}, {}, method1,       method2},
              {0.9, 0.9, *textArlrd, *textArlrd, 70, 50, 140,                     {0.1f, 0.1f, 0.7f}, {0.1f, 0.1f, 0.7f}, {}, method3,       method4},
              //move{},
              {1.2, 0.0, *textAlger, *textArlrd, 80, 0,  140,                     {0.1f, 0.7f, 0.1f}, {},                 {}, "Move",        ""},
              {0.9, 0.9, *textArlrd, *textArlrd, 50, 50, 140,                     {0.1f, 0.1f, 0.7f}, {0.1f, 0.1f, 0.7f}, {}, move1,         move2},
              //other{},
              {1.2, 0.9, *textAlger, *textArlrd, 80, 50, 140,                     {0.1f, 0.9f, 0.1f}, {0.1f, 0.1f, 0.7f}, {}, "Other",       other},
      };
      model = glm::mat4{1.0f};
      Shader &contextShader = shaders.at("context");
      contextShader.begin();
      while (process == des) {
            if (!app.update())process = qut;
            Call(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
            contextShader.begin();
            contextShader.setMatrix4x4("projection", glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height)));
            contextShader.setMatrix4x4("model", model);
            menu.renderContext(contextShader, 80, messages);
            contextShader.end();
      }
      model = glm::mat4{1.0f};
}

void Game::recordsProcess() {
      static double modelM{0.0};
      app.setOnKeyCallBack([](int key, int scancode, int action, int mods) {
          if (key == GLFW_KEY_ESCAPE)process = menus;
      });
      app.setOnScrollCallback([](double x, double y) {
          modelM -= y * 35;
      });
      app.setOnCursorPosCallback(nullptr);
      std::vector<Message> messages{
              {1.4, 0.0, *textGorte, *textAlger, 0, 20, width / 2 - 7 * 40 / 2, {0.0f, 1.0f, 0.0f}, {}, {}, "Records", ""},
      };
      std::vector<GameRecord> recordSet;
      std::vector<GameRecord> records = gameRecorder.loadRecords();
      for (auto &record: records) {
            if (!record.complete)continue;
            bool has = false;
            for (auto &it: recordSet) {
                  if (it.torusNum == record.torusNum) {
                        has = true;
                        if (it.timeSpend > record.timeSpend) {
                              it.step = record.step;
                              it.timeSpend = record.timeSpend;
                        }
                  }
            }
            if (!has)recordSet.emplace_back(record);
      }
      std::sort(recordSet.begin(), recordSet.end(), [](const GameRecord &a, const GameRecord &b) -> int {
          return a.torusNum < b.torusNum;
      });
      for (const auto &item: recordSet) {
            using namespace std;
            messages.push_back(
                    {0.9, 1.0, *textAlger, *textArlrd, 110, 55, 120, {0.1f, 0.8f, 0.1f}, {0.23f, 0.18f, 0.49f}, {},
                     "Records:" + item.recordName,
                     "Disks : " + to_string(item.torusNum) + "   TimeSpend :   " + to_string(item.timeSpend) + " sec   " + "Steps: " +
                     to_string(item.step)}
            );
      }
      Shader &contextShader = shaders.at("context");
      Shader &skyboxShader = shaders.at("skybox");
      contextShader.begin();
      while (process == rec) {
            if (!app.update())process = qut;
            Call(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
            contextShader.begin();
            contextShader.setMatrix4x4("projection", glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height)));
            if (modelM > 190 * recordSet.size())
                  modelM = 190 * recordSet.size();
            if (modelM < 0)modelM = 0;
            model = glm::translate(glm::mat4{1.0f}, {0.0f, modelM, 0.0f});
            contextShader.setMatrix4x4("model", model);
            menu.renderContext(contextShader, 80, messages);
            contextShader.end();
      }
      modelM = 0.0;
      model = glm::mat4{1.0f};
      app.setOnScrollCallback(nullptr);
}

void Game::oldGameProcess() {
      static int cho{0};
      static double modelM{0.0};
      app.setOnKeyCallBack([](int key, int scancode, int action, int mods) {
          if (key == GLFW_KEY_ESCAPE)process = menus;
          else if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)cho++;
          else if (key == GLFW_KEY_UP && action == GLFW_RELEASE)cho--;
          else if (key == GLFW_KEY_ENTER && action == GLFW_RELEASE) {
                process = sgm;
          }
      });
      app.setOnScrollCallback([](double x, double y) {
          modelM -= y * 35;
      });
      app.setOnCursorPosCallback(nullptr);
      std::vector<Message> messages{
              {1.4, 0.0, *textGorte, *textAlger, 0, 20, width / 2 - 13 * 40 / 2, {0.0f, 1.0f, 0.0f}, {}, {}, "History Games", ""},
      };
      std::vector<GameRecord> recordSet;
      std::vector<GameRecord> records = gameRecorder.loadRecords();
      for (auto &record: records) {
            if (record.complete)continue;
            recordSet.emplace_back(record);
      }
      for (const auto &item: recordSet) {
            using namespace std;
            messages.push_back(
                    {0.9, 1.0, *textAlger, *textArlrd, 110, 55, 120, {0.1f, 0.8f, 0.1f}, {0.23f, 0.18f, 0.49f}, {},
                     "Game :" + item.recordName,
                     "Disks : " + to_string(item.torusNum) + "   TimeSpend :   " + to_string(item.timeSpend) + " sec   " + "Steps: " +
                     to_string(item.step)}
            );
      }
      Shader &contextShader = shaders.at("context");
      Shader &skyboxShader = shaders.at("skybox");
      contextShader.begin();
      while (process == ogm || process == sgm) {
            if (cho < 0)cho = 0;
            if (cho > static_cast<int>(recordSet.size()) - 1)cho = static_cast<int>(recordSet.size()) - 1;
            for (auto &item: messages)item.light = {};
            if (process == sgm) {
                  startOldGame(recordSet[cho]);
            }
            messages[cho + 1].light = {0.2f, 0.2f, 0.2f};
            if (!app.update())process = qut;
            Call(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
            contextShader.begin();
            contextShader.setMatrix4x4("projection", glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height)));
            if (modelM > 190 * recordSet.size())
                  modelM = 190 * recordSet.size();
            if (modelM < 0)modelM = 0;
            model = glm::translate(glm::mat4{1.0f}, {0.0f, modelM, 0.0f});
            contextShader.setMatrix4x4("model", model);
            menu.renderContext(contextShader, 80, messages);
            contextShader.end();
      }
      modelM = 0.0;
      model = glm::mat4{1.0f};
      app.setOnScrollCallback(nullptr);
}

void Game::preGamePare() {
      step = 0;
      prepareTable();
      prepareOptions();
      camera->refresh();
      refreshOptionPositions();
      glfwSetCursorPos(app.window, width / 2.0, height / 2.0);
      app.setOnKeyCallBack(inGameKeyCallBack);
      app.setOnCursorPosCallback(inGameCursorCallBack);
      camera->mouseCall(-14, -120, true);
      renderPrompt = [](const Shader &textShader, const Text &text) {
          std::string s = "Use  W,A,S,D keys to Change the view";
          std::string s2 = "Or use Mouse with right button holding down";
          std::string s3 = "Press C to close this Prompt";
          text.renderText(textShader, s, {width / 2 - s.length() * 36 / 2, height / 2 + 200}, 1.4, {0.8f, 0.1f, 0.1f});
          text.renderText(textShader, s2, {width / 2 - s2.length() * 32 / 2, height / 2 + 120}, 1.3, {0.8f, 0.1f, 0.1f});
          text.renderText(textShader, s3, {width / 2 - s3.length() * 32 / 2, height / 2 + 20}, 1.3, {0.8f, 0.1f, 0.1f});
      };
}

void Game::startNewGame(int levels) {
      preGamePare();
      level = levels;
      prepareTours();
      pillar_height = static_cast<float>(level) * torus_radius * 2 + 0.4f;
      preparePillars();
      startTime = std::chrono::system_clock::now();
      while (process == ngm) {
            if (!app.update())process = qut;
            app.titleFps();
            inGameRender();
            inGameProcessCursor(app);
            inGameProcessKeyInput(app);
      }
      using namespace std::chrono;
      long long timeSpend = duration_cast<seconds>((system_clock::now() - startTime)).count();
      GameRecord gameRecord{};
      gameRecord.recordName = "";
      if (process == win) {
            std::vector<GameRecord> recordSet;
            std::vector<GameRecord> records = gameRecorder.loadRecords();
            records = gameRecorder.loadRecords();
            for (auto &record: records) {
                  if (!record.complete)continue;
                  bool has = false;
                  for (auto &it: recordSet) {
                        if (it.torusNum == record.torusNum) {
                              has = true;
                              if (it.timeSpend > record.timeSpend) {
                                    it.step = record.step;
                                    it.timeSpend = record.timeSpend;
                              }
                        }
                  }
                  if (!has)recordSet.emplace_back(record);
            }
            int num = 0;
            for (const auto &item: recordSet) {
                  if (item.torusNum == level) {
                        num++;
                        gameRecord = item;
                  }
            }
            if (num == 1)gameRecord.recordName = "";
      }
      while (process == win) {
            if (!app.update())process = qut;
            app.titleFps();
            inGameRender();
            renderWinMessage(timeSpend, gameRecord);
            inGameProcessCursor(app);
            inGameProcessKeyInput(app);
      }
      endGameClear();
}

void Game::startOldGame(const GameRecord &record) {
      preGamePare();
      level = record.torusNum;
      prepareTours(record);
      pillar_height = static_cast<float>(level) * torus_radius * 2 + 0.4f;
      preparePillars();
      step = record.step;
      startTime = std::chrono::system_clock::now() + std::chrono::seconds(record.timeSpend);
      while (process == sgm) {
            if (!app.update())process = qut;
            app.titleFps();
            inGameRender();
            inGameProcessCursor(app);
            inGameProcessKeyInput(app);
      }
      using namespace std::chrono;
      long long timeSpend = duration_cast<seconds>((system_clock::now() - startTime)).count();
      GameRecord gameRecord{};
      gameRecord.recordName = "";
      if (process == win) {
            std::vector<GameRecord> recordSet;
            std::vector<GameRecord> records = gameRecorder.loadRecords();
            records = gameRecorder.loadRecords();
            for (auto &record_: records) {
                  if (!record_.complete)continue;
                  bool has = false;
                  for (auto &it: recordSet) {
                        if (it.torusNum == record_.torusNum) {
                              has = true;
                              if (it.timeSpend > record_.timeSpend) {
                                    it.step = record_.step;
                                    it.timeSpend = record_.timeSpend;
                              }
                        }
                  }
                  if (!has)recordSet.emplace_back(record_);
            }
            int num = 0;
            for (const auto &item: recordSet) {
                  if (item.torusNum == level) {
                        num++;
                        gameRecord = item;
                  }
            }
            if (num == 1)gameRecord.recordName = "";
      }
      while (process == win) {
            gameRecorder.removeRecord(record.recordName);
            if (!app.update())process = qut;
            app.titleFps();
            inGameRender();
            renderWinMessage(timeSpend, gameRecord);
            inGameProcessCursor(app);
            inGameProcessKeyInput(app);
      }
      endGameClear();
}


void Game::presentation() {
      preGamePare();
      level = 4;
      prepareTours();
      pillar_height = static_cast<float>(level) * torus_radius * 2 + 0.4f;
      preparePillars();
      startTime = std::chrono::system_clock::now();
      auto a = std::async(presentationProcessor, std::ref(torusL), std::ref(torusM), std::ref(torusR), &Game::move, std::ref(*this));
      app.setOnKeyCallBack(nullptr);
      while (process == demo) {
            if (!app.update())process = qut;
            app.titleFps();
            inGameRender();
            inGameProcessKeyInput(app);
      }
      app.setOnKeyCallBack([](int key, int scancode, int action, int mods) {
          if (key == GLFW_KEY_ESCAPE)process = menus;
      });
      using namespace std::chrono;
      long long timeSpend = duration_cast<seconds>((system_clock::now() - startTime)).count();
      while (process == win) {
            if (!app.update())process = qut;
            app.titleFps();
            inGameRender();
            renderWinMessage(timeSpend, {{}, {}, {}, {}, ""});
            inGameProcessCursor(app);
            inGameProcessKeyInput(app);
      }
      endGameClear();
}

void Game::endGameClear() {
      step = 0;
      level = 0;
      torusL.clear();
      torusM.clear();
      torusR.clear();
      lastX = static_cast<float>(width / 2.0);
      lastY = static_cast<float>(height / 2.0);
      camera->refresh();
      app.setOnKeyCallBack(nullptr);
      app.setOnCursorPosCallback(nullptr);
      app.setInputMode(GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Game::preparePillars(){
      Texture pillarTexture1{"Assets/Images/pblue.png", ++textureUnit};
      Texture pillarTexture2{"Assets/Images/plime.png", ++textureUnit};
      Texture pillarTexture3{"Assets/Images/pred.png", ++textureUnit};
      pillarL = std::make_shared<Pillar>(Pillar{pillarTexture1});
      pillarL->generateMesh(slices, pillar_height, pillar_radius, .0f);
      pillarL->setup();
      pillarM = std::make_shared<Pillar>(Pillar{pillarTexture2});
      pillarM->generateMesh(slices, pillar_height, pillar_radius, .0f);
      pillarM->setup();
      pillarR = std::make_shared<Pillar>(Pillar{pillarTexture3});
      pillarR->generateMesh(slices, pillar_height, pillar_radius, .0f);
      pillarR->setup();
}


void Game::prepareTours(const GameRecord &record) {
      std::vector<string> images = {
              "blue.png", "cyan.png", "green.png", "lime.png", "orange.png", "purple.png", "yellow.png", "pink.png",
              "top.png",
      };
      std::vector<Texture> textures;
      for (const auto &item: images) {
            textures.emplace_back("Assets/Images/" + item, ++textureUnit);
      }

      std::default_random_engine e{};
      e.seed(time(nullptr));
      std::uniform_int_distribution<int> u{0, (int) time(nullptr)}; // 左闭右闭区间
      int dex = u(e);
      for (const auto &item: record.torus[0]) {
            Torus torus{textures[(dex + item) % textures.size()], item};
            torus.generateMesh(slices, min_torus_inner_radius + (float) item * radius_increase,
                               min_torus_inner_radius + (float) item * radius_increase + torus_radius * 2,
                               0.0f);
            torus.setup();
            torusL.emplace_back(std::move(torus));
      }
      for (const auto &item: record.torus[1]) {
            Torus torus{textures[(dex + item) % textures.size()], item};
            torus.generateMesh(slices, min_torus_inner_radius + (float) item * radius_increase,
                               min_torus_inner_radius + (float) item * radius_increase + torus_radius * 2,
                               0.0f);
            torus.setup();
            torusM.emplace_back(std::move(torus));
      }
      for (const auto &item: record.torus[2]) {
            Torus torus{textures[(dex + item) % textures.size()], item};
            torus.generateMesh(slices, min_torus_inner_radius + (float) item * radius_increase,
                               min_torus_inner_radius + (float) item * radius_increase + torus_radius * 2,
                               0.0f);
            torus.setup();
            torusR.emplace_back(std::move(torus));
      }
      glm::mat4 trans{1.0f};
      trans = glm::translate(trans, glm::vec3{cubePositions[0].x, 0.0f, 0.0f});
      models.resize(level);
      for (auto it = record.torus[0].rbegin(); it != record.torus[0].rend(); it++) {
            trans = glm::translate(trans, glm::vec3{0.0f, torus_radius * 2, 0.0f});
            models[*it] = trans;
      }
      trans = glm::translate(glm::mat4{1.0f}, glm::vec3{cubePositions[1].x, 0.0f, 0.0f});
      for (auto it = record.torus[1].rbegin(); it != record.torus[1].rend(); it++) {
            trans = glm::translate(trans, glm::vec3{0.0f, torus_radius * 2, 0.0f});
            models[*it] = trans;
      }
      trans = glm::translate(glm::mat4{1.0f}, glm::vec3{cubePositions[2].x, 0.0f, 0.0f});
      for (auto it = record.torus[2].rbegin(); it != record.torus[2].rend(); it++) {
            trans = glm::translate(trans, glm::vec3{0.0f, torus_radius * 2, 0.0f});
            models[*it] = trans;
      }
}


void Game::prepareTours() {
      std::vector<string> images = {
              "blue.png", "cyan.png", "green.png", "lime.png", "orange.png", "purple.png", "yellow.png", "pink.png",
              "top.png",
      };
      std::vector<Texture> textures;
      for (const auto &item: images) {
            textures.emplace_back("Assets/Images/" + item, ++textureUnit);
      }

      std::default_random_engine e{};
      e.seed(time(nullptr));
      std::uniform_int_distribution<int> u{0, (int) time(nullptr)}; // 左闭右闭区间
      int dex = u(e);
      for (int i = 0; i < level; ++i) {
            Torus torus{textures[(dex + i) % textures.size()], i};
            torus.generateMesh(slices, min_torus_inner_radius + i * radius_increase,
                               min_torus_inner_radius + i * radius_increase + torus_radius * 2,
                               0.0f);
            torus.setup();
            torusL.emplace_back(std::move(torus));
      }

      glm::mat4 trans{1.0f};
      trans = glm::translate(trans, glm::vec3{cubePositions[0].x, 0.0f, 0.0f});
      models.resize(level);
      for (int i = level - 1; i > -1; --i) {
            trans = glm::translate(trans, glm::vec3{0.0f, torus_radius * 2, 0.0f});
            models[i] = trans;
      }
}

void Game::prepareTable() {
      Texture tableTexture{"Assets/Images/table.png", ++textureUnit};
      table = std::make_shared<Table>(Table{tableTexture});
      table->generateMesh(0.0f, table_length, table_width, table_height);
      table->setup();
}

void Game::prepareOptions() {
      using ss = status::Status;
      float width_ = static_cast<float>(width) / 4;
      glm::vec3 color = {0.3f, 0.3f, 0.95f};
      glm::vec3 colorM = {0.23f, 0.18f, 0.49f};
      options.insert({"A", {1.5, color, {width_ * 1, button_h}, ss::released, "A"}});
      options.insert({"B", {1.5, color, {width_ * 2, button_h}, ss::released, "B"}});
      options.insert({"C", {1.5, color, {width_ * 3, button_h}, ss::released, "C"}});
      options.insert({"MA", {0.8, colorM, {width_ * 1 - 60, button_h - 100}, ss::released, "Move to A"}});
      options.insert({"MB", {0.8, colorM, {width_ * 2 - 60, button_h - 100}, ss::released, "Move to B"}});
      options.insert({"MC", {0.8, colorM, {width_ * 3 - 60, button_h - 100}, ss::released, "Move to C"}});
}

/**
 * TODO: menu.options
 * */
void Game::refreshOptionPositions() {
      float width_ = static_cast<float>(width) / 4;
      if (!options.empty()) {
            options.at("A").position = {width_ * 1, button_h};
            options.at("B").position = {width_ * 2, button_h};
            options.at("C").position = {width_ * 3, button_h};
            options.at("MA").position = {width_ * 1 - 60, button_h - 100};
            options.at("MB").position = {width_ * 2 - 60, button_h - 100};
            options.at("MC").position = {width_ * 3 - 60, button_h - 100};
      }
      menu = Menu{width, height};
}

inline void Game::inGameRender() {
      Call(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
      Shader textShader = shaders.at("text");
      Shader &inGameShader = shaders.at("inGame");
      Shader &skyboxShader = shaders.at("skybox");
      skyboxShader.begin();
      inGameBack->render(skyboxShader, *camera);
      skyboxShader.end();
      inGameShader.begin();
      inGameShader.setMatrix4x4("viewMatrix", camera->getViewMatrix());
      renderTable(inGameShader);
      renderPillars(inGameShader);
      renderTorus(inGameShader);
      inGameShader.end();
      textShader.begin();
      renderText(textShader);
      textShader.end();

}

void Game::renderTable(const Shader &inGameShader) {
      model = glm::translate(glm::mat4{1.0f}, glm::vec3{0.0f, torus_radius, 0.0f});
      inGameShader.setMatrix4x4("model", model);
      inGameShader.setInt("pictureSampler", static_cast<int>(table->get_texture().get_unit()));
      inGameShader.setVector3("light", 0.0f, 0.0f, 0.0f);
      table->render(inGameShader);
}

void Game::renderPillars(const Shader &inGameShader) {
      model = glm::translate(glm::mat4{1.0f}, cubePositions[0]);
      inGameShader.setMatrix4x4("model", model);
      inGameShader.setInt("pictureSampler", static_cast<int>(pillarL->get_texture().get_unit()));
      inGameShader.setVector3("light", pillarL->get_light(), pillarL->get_light(), pillarL->get_light());
      pillarL->render(inGameShader);
      model = glm::translate(glm::mat4{1.0f}, cubePositions[1]);
      inGameShader.setMatrix4x4("model", model);
      inGameShader.setInt("pictureSampler", static_cast<int>(pillarM->get_texture().get_unit()));
      inGameShader.setVector3("light", pillarM->get_light(), pillarM->get_light(), pillarM->get_light());
      pillarM->render(inGameShader);
      model = glm::translate(glm::mat4{1.0f}, cubePositions[2]);
      inGameShader.setMatrix4x4("model", model);
      inGameShader.setInt("pictureSampler", static_cast<int>(pillarR->get_texture().get_unit()));
      inGameShader.setVector3("light", pillarR->get_light(), pillarR->get_light(), pillarR->get_light());
      pillarR->render(inGameShader);
}

void Game::renderTorus(const Shader &inGameShader) {
      for (auto &item: torusL) {
            inGameShader.setMatrix4x4("model", models[item.get_index()]);
            inGameShader.setInt("pictureSampler", static_cast<int>(item.get_texture().get_unit()));
            inGameShader.setVector3("light", item.get_light(), item.get_light(), item.get_light());
            item.render(inGameShader);
      }
      for (auto &item: torusM) {
            inGameShader.setMatrix4x4("model", models[item.get_index()]);
            inGameShader.setInt("pictureSampler", static_cast<int>(item.get_texture().get_unit()));
            inGameShader.setVector3("light", item.get_light(), item.get_light(), item.get_light());
            item.render(inGameShader);
      }
      for (auto &item: torusR) {
            inGameShader.setMatrix4x4("model", models[item.get_index()]);
            inGameShader.setInt("pictureSampler", static_cast<int>(item.get_texture().get_unit()));
            inGameShader.setVector3("light", item.get_light(), item.get_light(), item.get_light());
            item.render(inGameShader);
      }
}

void Game::renderText(const Shader &textShader) {
      using namespace std::chrono;
      using os = status::OptionState;
      static long long ex = duration_cast<seconds>((system_clock::now() - startTime)).count();
      if (process != win)ex = duration_cast<seconds>((system_clock::now() - startTime)).count();
      textShader.setMatrix4x4("projection", glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height)));
      textAlger->renderText(textShader, "Time Has Spend: " + std::to_string(ex) + "  sec    Steps: " + std::to_string(step), {10, height - 50}, 1,
                            {0.82f, 0.26f, 0.26f});
      for (const auto &item: options) {
            auto &option = item.second;
            textAlger->renderText(textShader, option.context, option.position, option.scale, option.color + os::getLight(option.status));
      }
      if (renderPrompt) {
            bool static fresh = true;
            auto static start = steady_clock::now();
            if (fresh) {
                  start = steady_clock::now();
                  fresh = false;
            }
            renderPrompt(textShader, *textArlrd);
            if (!got)start = steady_clock::now();
            long long e = duration_cast<seconds>((steady_clock::now() - start)).count();
            if (e >= 2) {
                  renderPrompt = nullptr;
                  fresh = true;
            }
      }
      if (renderWarning) {
            bool static fresh = true;
            auto static start = steady_clock::now();
            if (fresh) {
                  start = steady_clock::now();
                  fresh = false;
            }
            renderWarning(textShader, *textArlrd);
            long long e = duration_cast<seconds>((steady_clock::now() - start)).count();
            if (e >= 1) {
                  renderWarning = nullptr;
                  fresh = true;
            }
      }
}

void Game::renderWinMessage(long long timeSpend, const GameRecord &record) {
      Shader &textShader = shaders.at("text");
      textShader.begin();
      textShader.setMatrix4x4("projection", glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height)));
      string s1 = "Congratulations";
      string s2 = "You have moved all the Disks in  ";
      string s3 = "  Seconds";
      string s4 = "New Record !";
      string s5 = "Your best record is ";
      textGorte->renderText(textShader, s1, {width / 2 - s1.length() * 44 / 2, height / 2 + 170}, 1.5, {0.8f, 0.3f, 0.4f});
      textArlrd->renderText(textShader, s2 + std::to_string(timeSpend) + s3,
                            {width / 2 - (s2.length() + s3.length() + std::to_string(timeSpend).length()) * 25 / 2, height / 2 + 90}, 1,
                            {0.3f, 0.7f, 0.4f});
      if (record.recordName.empty()) {
            textAlger->renderText(textShader, s4,
                                  {width / 2 - s4.length() * 46 / 2, height / 2 - 60}, 1.8,
                                  {1.0f, 0.5f, 0.5f});
      } else {
            if (record.timeSpend > timeSpend) {
                  textAlger->renderText(textShader, s4,
                                        {width / 2 - s4.length() * 46 / 2, height / 2 - 60}, 1.8,
                                        {1.0f, 0.6f, 0.6f});
            } else {
                  textArlrd->renderText(textShader, s5 + std::to_string(record.timeSpend),
                                        {width / 2 - (s5.length() + std::to_string(record.timeSpend).length()) * 46 / 2, height / 2 - 60}, 1.8,
                                        {0.9f, 0.4f, 0.4f});
            }
      }
      textShader.end();
}

void Game::move(std::vector<Torus> *from, std::vector<Torus> &to) {
      if (!from)return;
      if (from == &to)return;
      if (from->empty()) {
            renderWarning = [](const Shader &textShader, const Text &text) {
                string s = "Nothing to Move";
                text.renderText(textShader, s, {width / 2 - s.length() * 48 / 2, height / 2 + 20}, 2, {0.8f, 0.1f, 0.1f});
            };
            return;
      }
      if (!to.empty() && (*from)[0].get_index() > to[0].get_index()) {
            renderWarning = [](const Shader &textShader, const Text &text) {
                string s = "Can not Move";
                text.renderText(textShader, s, {width / 2 - s.length() * 48 / 2, height / 2 + 20}, 2, {0.8f, 0.1f, 0.1f});
            };
      } else {
            step++;
            if (!to.empty()) {
                  auto p = models[to[0].get_index()];
                  models[(*from)[0].get_index()] = glm::translate(p, glm::vec3{0.0f, torus_radius * 2, 0.0f});
            } else {
                  glm::mat4 trans{1.0f};
                  if (&to == &torusL) {
                        trans = glm::translate(trans, glm::vec3{cubePositions[0].x, 0.0f, 0.0f});
                  } else if (&to == &torusM) {
                        trans = glm::translate(trans, glm::vec3{cubePositions[1].x, 0.0f, 0.0f});
                  } else {
                        trans = glm::translate(trans, glm::vec3{cubePositions[2].x, 0.0f, 0.0f});
                  }
                  trans = glm::translate(trans, glm::vec3{0.0f, torus_radius * 2, 0.0f});
                  models[(*from)[0].get_index()] = trans;
            }
            to.push_back((*from)[0]);
            std::sort(to.begin(), to.end(), [](const Torus &a, const Torus &b) -> int {
                return a.get_index() < b.get_index();
            });
            from->erase(from->begin());
      }
      if (torusR.size() == level) {
            auto now = std::chrono::system_clock::now();
            std::time_t now_c = std::chrono::system_clock::to_time_t(now);
            struct tm now_tm = *std::localtime(&now_c);
            std::ostringstream os;
            os << std::put_time(&now_tm, "%Y-%m-%d-%H:%M:%S");
            auto time = std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count();
            std::vector<int> torus;
            for (int i = 0; i < level; ++i) {
                  torus.push_back(i);
            }
            if (process != demo)
                  gameRecorder.addRecord(
                          {step, level, true, time,
                           os.str(),
                           {{}, {}, torus}}
                  );
            process = win;
      }
      chosen = nullptr;
}