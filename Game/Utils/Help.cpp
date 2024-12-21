#include "Help.h"

//广搜用
void move(std::vector<int> *max, std::vector<int> *middle, std::vector<int> *min, std::string ma, std::string mid, std::string mi,
          std::queue<HanoiStatus> &states, HanoiStatus &state) {
      middle->insert(middle->begin(), (*min)[0]);
      min->erase(min->begin());
      states.push({std::string(state.path).append(mi + "==>" + mid + ", "), state.l, state.m, state.r});

      max->insert(max->begin(), (*middle)[0]);
      middle->erase(middle->begin());
      states.push({std::string(state.path).append(mi + "==>" + ma + ", "), state.l, state.m, state.r});

      min->insert(min->begin(), (*max)[0]);
      (*max)[0] = (*middle)[0];
      middle->erase(middle->begin());
      states.push({std::string(state.path).append(mid + "==>" + ma + ", "), state.l, state.m, state.r});
}

//深搜用
std::string move(std::vector<int> *max, std::vector<int> *middle, std::vector<int> *min, std::string ma, std::string mid, std::string mi,
                 HanoiStatus &state) {
      middle->insert(middle->begin(), (*min)[0]);
      min->erase(min->begin());
      std::string s = searchDFS({std::string(state.path).append(mi + "==>" + mid + ", "), state.l, state.m, state.r});
      if (!s.empty())return s;

      max->insert(max->begin(), (*middle)[0]);
      middle->erase(middle->begin());
      s = searchDFS({std::string(state.path).append(mi + "==>" + ma + ", "), state.l, state.m, state.r});
      if (!s.empty())return s;

      min->insert(min->begin(), (*max)[0]);
      (*max)[0] = (*middle)[0];
      middle->erase(middle->begin());
      s = searchDFS({std::string(state.path).append(mid + "==>" + ma + ", "), state.l, state.m, state.r});
      if (!s.empty())return s;
      return "";
}

int getHelp(std::string &path, bool *got, std::vector<int> l, std::vector<int> m, std::vector<int> r) {
      path = search({"", std::move(l), std::move(m), std::move(r)});//广搜
      std::cout << path;
      *got = true;
      return 1;
}

std::unordered_set<HanoiStatus, HanoiStatusHash, HanoiStatusEqual> searchDFSSet;

std::string search(const HanoiStatus &status_) {//广度优先搜索
      std::string ma, mid, mi;
      std::vector<int> *max;
      std::vector<int> *min;
      std::vector<int> *middle;
      std::queue<HanoiStatus> statuses;
      std::unordered_set<HanoiStatus, HanoiStatusHash, HanoiStatusEqual> searchSet;
      statuses.push(status_);
      while (!statuses.empty()) {
            int size = static_cast<int>(statuses.size());
            for (int i = 0; i < size; ++i) {
                  HanoiStatus status = statuses.front();
                  statuses.pop();
                  if (status.l.empty() && status.m.empty()) {
                        return status.path;
                  }
                  if (searchSet.find(status) != searchSet.end()) {
                        continue;
                  } else {
                        searchSet.insert(status);
                  }
                  if (status.l.empty() || status.m.empty() || status.r.empty()) {
                        //l m 不同时 empty
                        if (status.l.empty()) {
                              if (status.r.empty()) {
                                    status.l.insert(status.l.begin(), status.m[0]);
                                    status.m.erase(status.m.begin());
                                    statuses.push({std::string(status.path) + "B==>A, ", status.l, status.m,
                                                   status.r});

                                    status.r.insert(status.r.begin(), status.l[0]);
                                    status.l.erase(status.l.begin());
                                    statuses.push({std::string(status.path) + "B==>C, ", status.l, status.m,
                                                   status.r});
                              } else {
                                    if (status.m[0] > status.r[0]) {
                                          max = &status.l;
                                          middle = &status.m;
                                          min = &status.r;
                                          ma = 'A', mid = 'B', mi = 'C';
                                    } else {
                                          max = &status.l;
                                          middle = &status.r;
                                          min = &status.m;
                                          ma = 'A', mid = 'C', mi = 'B';
                                    }
                                    move(max, middle, min, ma, mid, mi, statuses, status);
                              }
                        } else if (status.m.empty()) {
                              if (status.r.empty()) {
                                    status.m.insert(status.m.begin(), status.l[0]);
                                    status.l.erase(status.l.begin());
                                    statuses.push({std::string(status.path) + "A==>B, ", status.l, status.m,
                                                   status.r});

                                    status.r.insert(status.r.begin(), status.m[0]);
                                    status.m.erase(status.m.begin());
                                    statuses.push({std::string(status.path) + "A==>C, ", status.l, status.m,
                                                   status.r});
                              } else {
                                    if (status.l[0] > status.r[0]) {
                                          max = &status.m;
                                          middle = &status.l;
                                          min = &status.r;
                                          ma = 'B', mid = 'A', mi = 'C';
                                    } else {
                                          max = &status.m;
                                          middle = &status.r;
                                          min = &status.l;
                                          ma = 'B', mid = 'C', mi = 'A';
                                    }
                                    move(max, middle, min, ma, mid, mi, statuses, status);
                              }
                        } else {
                              if (status.l[0] > status.m[0]) {
                                    max = &status.r;
                                    middle = &status.l;
                                    min = &status.m;
                                    ma = 'C', mid = 'A', mi = 'B';
                              } else {
                                    max = &status.r;
                                    middle = &status.m;
                                    min = &status.l;
                                    ma = 'C', mid = 'B', mi = 'A';
                              }
                              move(max, middle, min, ma, mid, mi, statuses, status);
                        }
                  } else {
                        if (status.l[0] > status.m[0]) {
                              if (status.m[0] > status.r[0]) {
                                    max = &status.l;
                                    middle = &status.m;
                                    min = &status.r;
                                    ma = 'A', mid = 'B', mi = 'C';
                              }
                              if (status.m[0] < status.r[0]) {
                                    if (status.l[0] > status.r[0]) {
                                          max = &status.l;
                                          middle = &status.r;
                                          min = &status.m;
                                          ma = 'A', mid = 'C', mi = 'B';
                                    } else {
                                          max = &status.r;
                                          middle = &status.l;
                                          min = &status.m;
                                          ma = 'C', mid = 'A', mi = 'B';
                                    }
                              }
                        } else {
                              if (status.l[0] > status.r[0]) {
                                    max = &status.m;
                                    middle = &status.l;
                                    min = &status.r;
                                    ma = 'B', mid = 'A', mi = 'C';
                              }
                              if (status.l[0] < status.r[0]) {
                                    if (status.m[0] > status.r[0]) {
                                          max = &status.m;
                                          middle = &status.r;
                                          min = &status.l;
                                          ma = 'B', mid = 'C', mi = 'A';
                                    } else {
                                          max = &status.r;
                                          middle = &status.m;
                                          min = &status.l;
                                          ma = 'C', mid = 'B', mi = 'A';
                                    }
                              }
                        }
                        move(max, middle, min, ma, mid, mi, statuses, status);
                  }
            }
      }
      return "";
}

void presentationProcessor(std::vector<Torus> &l, std::vector<Torus> &m, std::vector<Torus> &r,
                           void(Game::*move)(std::vector<Torus> *from, std::vector<Torus> &to), Game &game) {
      string s = "A==>B, A==>C, B==>C, A==>B, C==>A, C==>B, A==>B, A==>C, B==>C, B==>A, C==>A, B==>C, A==>B, A==>C, B==>C,";
      std::this_thread::sleep_for(std::chrono::milliseconds(2500));
      for (auto i = s.begin(); i < s.end();) {
            switch (*i) {
                  case 'A': {
                        l[0].chosen();
                        std::this_thread::sleep_for(std::chrono::milliseconds(500));
                        i += 4;
                        l[0].released();
                        switch (*i) {
                              case 'B':
                                    (game.*move)(&l, m);
                                    std::this_thread::sleep_for(std::chrono::milliseconds(900));
                                    break;
                              case 'C':
                                    (game.*move)(&l, r);
                                    std::this_thread::sleep_for(std::chrono::milliseconds(900));
                                    break;
                        }
                        break;
                  }
                  case 'B': {
                        m[0].chosen();
                        std::this_thread::sleep_for(std::chrono::milliseconds(500));
                        i += 4;
                        m[0].released();
                        switch (*i) {
                              case 'A':
                                    (game.*move)(&m, l);
                                    std::this_thread::sleep_for(std::chrono::milliseconds(900));
                                    break;
                              case 'C':
                                    (game.*move)(&m, r);
                                    std::this_thread::sleep_for(std::chrono::milliseconds(900));
                                    break;
                        }
                        break;
                  }
                  case 'C': {
                        r[0].chosen();
                        std::this_thread::sleep_for(std::chrono::milliseconds(500));
                        r[0].released();
                        i += 4;
                        switch (*i) {
                              case 'A':
                                    (game.*move)(&r, l);
                                    std::this_thread::sleep_for(std::chrono::milliseconds(900));
                                    break;
                              case 'B':
                                    (game.*move)(&r, m);
                                    std::this_thread::sleep_for(std::chrono::milliseconds(900));
                                    break;
                        }
                        break;
                  }
            }
            i += 3;
      }
}

std::string searchDFS(HanoiStatus status) {//深度优先搜索（未使用）
      if (status.l.empty() && status.m.empty()) {
            return status.path;
      }
      if (searchDFSSet.find(status) != searchDFSSet.end()) {
            status.path = "";
            return "";
      } else {
            searchDFSSet.insert(status);
      }
      HanoiStatus result;
      std::string ma, mid, mi;
      std::vector<int> *max;
      std::vector<int> *min;
      std::vector<int> *middle;
      if (status.l.empty() || status.m.empty() || status.r.empty()) {
            //l m 不同时 empty
            if (status.l.empty()) {
                  if (status.r.empty()) {
                        status.l.insert(status.l.begin(), status.m[0]);
                        status.m.erase(status.m.begin());
                        std::string s = searchDFS({std::string(status.path) + "B==>A, ", status.l, status.m, status.r});
                        if (!s.empty())return s;

                        status.r.insert(status.r.begin(), status.l[0]);
                        status.l.erase(status.l.begin());
                        s = searchDFS({std::string(status.path) + "B==>C, ", status.l, status.m, status.r});
                        if (!s.empty())return s;
                  } else {
                        if (status.m[0] > status.r[0]) {
                              max = &status.l;
                              middle = &status.m;
                              min = &status.r;
                              ma = 'A', mid = 'B', mi = 'C';
                        } else {
                              max = &status.l;
                              middle = &status.r;
                              min = &status.m;
                              ma = 'A', mid = 'C', mi = 'B';
                        }
                        std::string s = move(max, middle, min, ma, mid, mi, status);
                        if (!s.empty())return s;
                  }
            } else if (status.m.empty()) {
                  if (status.r.empty()) {
                        status.m.insert(status.m.begin(), status.l[0]);
                        status.l.erase(status.l.begin());
                        std::string s = searchDFS({std::string(status.path) + "A==>B, ", status.l, status.m,
                                                   status.r});
                        if (!s.empty())return s;

                        status.r.insert(status.r.begin(), status.m[0]);
                        status.m.erase(status.m.begin());
                        s = searchDFS({std::string(status.path) + "A==>C, ", status.l, status.m,
                                       status.r});
                        if (!s.empty())return s;
                  } else {
                        if (status.l[0] > status.r[0]) {
                              max = &status.m;
                              middle = &status.l;
                              min = &status.r;
                              ma = 'B', mid = 'A', mi = 'C';
                        } else {
                              max = &status.m;
                              middle = &status.r;
                              min = &status.l;
                              ma = 'B', mid = 'C', mi = 'A';
                        }
                        std::string s = move(max, middle, min, ma, mid, mi, status);
                        if (!s.empty())return s;
                  }
            } else {
                  if (status.l[0] > status.m[0]) {
                        max = &status.r;
                        middle = &status.l;
                        min = &status.m;
                        ma = 'C', mid = 'A', mi = 'B';
                  } else {
                        max = &status.r;
                        middle = &status.m;
                        min = &status.l;
                        ma = 'C', mid = 'B', mi = 'A';
                  }
                  std::string s = move(max, middle, min, ma, mid, mi, status);
                  if (!s.empty())return s;
            }
      } else {
            if (status.l[0] > status.m[0]) {
                  if (status.m[0] > status.r[0]) {
                        max = &status.l;
                        middle = &status.m;
                        min = &status.r;
                        ma = 'A', mid = 'B', mi = 'C';
                  }
                  if (status.m[0] < status.r[0]) {
                        if (status.l[0] > status.r[0]) {
                              max = &status.l;
                              middle = &status.r;
                              min = &status.m;
                              ma = 'A', mid = 'C', mi = 'B';
                        } else {
                              max = &status.r;
                              middle = &status.l;
                              min = &status.m;
                              ma = 'C', mid = 'A', mi = 'B';
                        }
                  }
            } else {
                  if (status.l[0] > status.r[0]) {
                        max = &status.m;
                        middle = &status.l;
                        min = &status.r;
                        ma = 'B', mid = 'A', mi = 'C';
                  }
                  if (status.l[0] < status.r[0]) {
                        if (status.m[0] > status.r[0]) {
                              max = &status.m;
                              middle = &status.r;
                              min = &status.l;
                              ma = 'B', mid = 'C', mi = 'A';
                        } else {
                              max = &status.r;
                              middle = &status.m;
                              min = &status.l;
                              ma = 'C', mid = 'B', mi = 'A';
                        }
                  }
            }
            std::string s = move(max, middle, min, ma, mid, mi, status);
            if (!s.empty())return s;
      }
      return "";
}
