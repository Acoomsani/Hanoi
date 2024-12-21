// @brief: 游戏主逻辑类

#include "General.h"
#include <Utils/TextUtil.h>
#include <Game/Utils/Text.h>
#include <Game/Utils/Help.h>
#include <Game/Components/Menu.h>
#include <Game/Components/Table.h>
#include <Game/Components/Torus.h>
#include <Game/Components/Pillar.h>
#include <Game/Components/Option.h>
#include <Game/Record/GameRecorder.h>
#include <Render/Application/Camera.h>
#include <Game/Components/Background.h>
#include <Game/Components/Background.h>
#include <Render/Application/Application.h>

#pragma once

class Game {
    //流程
    enum Process {
        qut = 0,
        menus,//菜单
        ngm,//新游戏
        ogm,//历史游戏
        des,//游戏描述
        rec,//游戏纪录
        win,//成功
        sgm,//开始历史游戏
        demo//游戏演示
    };
public:
    Game();

    ~Game();

    void start();

private:
    static void prepareOptions();//准备相关选项

    static void refreshOptionPositions();//刷新选项位置

    static void levelInputKeyCallBack(int key, int scancode, int action, int mods);//输入圆盘数量回调函数

    static void inGameKeyCallBack(int key, int scancode, int action, int mods);//游戏中键盘回调函数

    static void inGameProcessKeyInput(const Application &application);//游戏中处理键盘输入（用于连续输入，回调函数无法处理）

    static void inGameCursorCallBack(double xPosIn, double yPosIn, bool movable);//游戏中处理鼠标光标移动

private:
    Application &app{*Application::getApplication()};//Application采用单例模式

    constexpr static const glm::vec3 cubePositions[] = {
            glm::vec3(-1.8f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(1.8f, 0.0f, 0.0f),
    };//三根杆位置

    static constexpr int slices{100};//圆切片数
    static constexpr float zFar{100};//远平面
    static constexpr float zNear{0.1f};//近平面
    static constexpr int button_h{200};
    static constexpr float table_width{2.0f};
    static constexpr float table_length{5.0f};
    static constexpr float table_height{0.1f};
    static constexpr float torus_radius{0.1f};
    static constexpr float pillar_radius{0.1f};
    static constexpr float radius_increase{0.045f};//半径增量
    static constexpr float min_torus_inner_radius{0.1f};//圆盘起始半径

    static int step;//步数
    glm::mat4 view{};//视图矩阵
    static Menu menu;//菜单
    glm::mat4 model{};//模型矩阵
    TextUtil textUtil{};
    glm::mat4 projection{};//投影矩阵
    static std::future<int> paths;
    std::vector<glm::mat4> models;//圆盘位置数组
    std::shared_ptr<Table> table{};
    std::shared_ptr<Text> textGorte{};//字体
    std::shared_ptr<Text> textArlrd{};//字体
    std::shared_ptr<Text> textAlger{};//字体
    std::shared_ptr<Pillar> pillarL{};
    std::shared_ptr<Pillar> pillarR{};
    std::shared_ptr<Pillar> pillarM{};
    static GameRecorder gameRecorder;
    static std::vector<Torus> torusM;
    static std::vector<Torus> torusR;
    static std::vector<Torus> torusL;
    std::vector<Torus> *chosen{nullptr};//选中的圆盘
    static std::unique_ptr<Camera> camera;
    std::shared_ptr<BackGround> menuBack{};//菜单背景
    std::shared_ptr<BackGround> inGameBack{};//游戏背景
    std::unordered_map<std::string, Shader> shaders{};
    static std::chrono::system_clock::time_point startTime;
    static std::unordered_map<std::string, Option> options;

    static std::function<void(const Shader &, const Text &)> renderPrompt;//渲染提示std::function
    static std::function<void(const Shader &, const Text &)> renderWarning;//渲染警告std::function


private:
    /*
     * 准备
     * */
    void preparePillars();

    void prepareTours();

    void prepareTours(const GameRecord &record);

    void prepareTable();

    void prepareMatrix();

    void prepareShaders();

    void prepareBackground();

    void loadCharacters();

    /*
     * */
    void inGameRender();//游戏中渲染

    void menuProcess();//菜单处理

    bool renderMenuLevelInput();

    void descriptionProcess();

    void recordsProcess();

    void oldGameProcess();

    void preGamePare();//游戏前准备

    void startNewGame(int levels);

    void startOldGame(const GameRecord &record);

    void presentation();

    void endGameClear();//结束一次游戏善后

    /*
     * 顾名思义
     * */
    void renderText(const Shader &textShader);

    void renderWinMessage(long long timeSpend, const GameRecord &record);

    void renderTable(const Shader &inGameShader);

    void renderPillars(const Shader &inGameShader);

    void renderTorus(const Shader &inGameShader);

    void menuProcessCursor(const Application &application);

    void inGameProcessCursor(const Application &application);

private:
    void releaseTorus();

    static void releaseOptions();

    void pillarChoices(double xPos);

    void torusMoveChoices(double xPos);

    void move(std::vector<Torus> *from, std::vector<Torus> &to);

private:
    /*
     * cpp文件中解释
     * */
    static bool got;
    static int level;
    static int width;
    static int height;
    static float fov;
    static float lastX;
    static float lastY;
    static std::string s;
    static float deltaTime;
    static float lastFrame;
    static bool firstMouse;
    static Process process;
    static float pillar_height;
    static unsigned int textureUnit;
    static unsigned int characterUnit;
};