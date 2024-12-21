// @brief: 获取提示相关函数

#include <Game/Components/Torus.h>
#include "General.h"

#pragma once

class Game;

struct HanoiStatus {
    std::string path;
    std::vector<int> l;
    std::vector<int> m;
    std::vector<int> r;
};

struct HanoiStatusHash {
    std::size_t operator()(const HanoiStatus &item) const {
          std::size_t h = 0;
          for (int i: item.l) { h ^= std::hash<int>()(i); }
          for (int i: item.m) { h ^= std::hash<int>()(i); }
          for (int i: item.r) { h ^= std::hash<int>()(i); }
          return h;
    }
};

struct HanoiStatusEqual {
    bool operator()(const HanoiStatus &a, const HanoiStatus &b) const {
          return a.l == b.l && a.m == b.m;
    }
};

void move(std::vector<int> *max, std::vector<int> *middle, std::vector<int> *min, std::string ma, std::string mid, std::string mi,
          std::queue<HanoiStatus> &states, HanoiStatus &state);

std::string search(const HanoiStatus &status_);

std::string searchDFS(HanoiStatus status);

int getHelp(std::string &path, bool *got, std::vector<int> l, std::vector<int> m, std::vector<int> r);

void presentationProcessor(std::vector<Torus> &l, std::vector<Torus> &m, std::vector<Torus> &r,
                           void(Game::*move)(std::vector<Torus> *from, std::vector<Torus> &to), Game &game);