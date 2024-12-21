// @brief: 记录游戏到文件

#include "General.h"

#pragma once

struct GameRecord {
    int step;
    int torusNum;
    bool complete;
    long long timeSpend;
    std::string recordName;
    std::vector<std::vector<int>> torus;

    bool operator==(const GameRecord &record) const;

    bool operator<(const GameRecord &record) const;

    friend std::istream &operator>>(std::istream &istream, GameRecord &record);

    friend std::ostream &operator<<(std::ostream &ostream, const GameRecord &record);
};

class GameRecorder {
public:
    GameRecorder(std::string path);

    bool addRecord(const GameRecord &record);

    bool removeRecord(const std::string &recordName);

    std::vector<GameRecord> loadRecords();

private:
    std::string path;
};