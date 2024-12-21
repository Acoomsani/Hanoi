#include "GameRecorder.h"

#include <utility>

std::ostream &operator<<(std::ostream &ostream, const GameRecord &record) {
      ostream
              << record.recordName << " " << record.timeSpend << " " << record.complete << " " << record.torusNum << " " << record.step << std::endl;
      for (const auto &toru: record.torus) {
            ostream << toru.size();
            for (int j = 0; j < toru.size(); ++j) {
                  ostream << " " << toru[j];
            }
            ostream << std::endl;
      }
      return ostream;
}

std::istream &operator>>(std::istream &istream, GameRecord &record) {
      istream >> record.recordName >> record.timeSpend >> record.complete >> record.torusNum >> record.step;
      record.torus.resize(3);
      int size;
      for (auto &toru: record.torus) {
            istream >> size;
            toru.resize(size);
            for (int j = 0; j < size; ++j) {
                  istream >> toru[j];
            }
      }
      return istream;
}

bool GameRecord::operator==(const GameRecord &record) const {
      return record.torusNum == this->torusNum
             && record.timeSpend == this->timeSpend;
}

bool GameRecord::operator<(const GameRecord &record) const {
      if (record.torusNum == this->torusNum)return record.timeSpend < this->timeSpend;
      return record.torusNum < this->torusNum;
}

GameRecorder::GameRecorder(std::string path) : path(std::move(path)) {
}

bool GameRecorder::addRecord(const GameRecord &record) {
      try {
            int size;
            std::fstream fs;
            std::vector<GameRecord> records;
            fs.open(path, std::ios_base::in);
            if (fs.peek() == std::ifstream::traits_type::eof()) {
                  size = 0;
                  fs.seekg(0);
            } else {
                  fs >> size;
            }
            records.resize(size);
            for (int i = 0; i < size; ++i) {
                  fs >> records[i];
            }
            fs.close();
            fs.open(path, std::ios_base::out | std::ios_base::trunc);
            records.emplace_back(record);
            fs << records.size() << std::endl;
            for (const auto &item: records) {
                  fs << item;
            }
            fs.flush();
            fs.close();
      } catch (std::exception &e) {
            return false;
      }
      return true;
}

std::vector<GameRecord> GameRecorder::loadRecords() {
      std::vector<GameRecord> records;
      try {
            int size;
            std::fstream fs;
            fs.open(path, std::ios_base::in);
            if (fs.peek() == std::ifstream::traits_type::eof()) {
                  size = 0;
                  fs.seekg(0);
            } else {
                  fs >> size;
            }
            records.resize(size);
            for (int i = 0; i < size; ++i) {
                  fs >> records[i];
            }
            fs.close();
      } catch (std::exception &e) {
            throw e;
      }
      return records;
}

bool GameRecorder::removeRecord(const std::string &recordName) {
      try {
            int size;
            std::fstream fs;
            std::vector<GameRecord> records;
            fs.open(path, std::ios_base::in);
            if (fs.peek() == std::ifstream::traits_type::eof()) {
                  size = 0;
                  fs.seekg(0);
            } else {
                  fs >> size;
            }
            records.resize(size);
            for (int i = 0; i < size; ++i) {
                  fs >> records[i];
            }
            fs.close();
            fs.open(path, std::ios_base::out | std::ios_base::trunc);
            bool has = false;
            for (auto it = records.begin(); it != records.end(); it++) {
                  if (it->recordName == recordName) {
                        has = true;
                        records.erase(it);
                  }
            }
            fs << records.size() << std::endl;
            for (const auto &item: records) {
                  fs << item;
            }
            fs.flush();
            fs.close();
            if (!has)return false;
      } catch (std::exception &e) {
            return false;
      }
      return true;
}


