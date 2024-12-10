#include <algorithm>
#include <cassert>
#include <cmath>
#include <deque>
#include <fstream>
#include <iostream>
#include <queue>
#include <unordered_set>
#include <vector>

struct Path {
  std::string state;
  std::string path;
};

static const std::string NONE = "NONE";

const size_t FIELD_WIDTH = 3;
const size_t FIELD_SIZE = FIELD_WIDTH * FIELD_WIDTH;
const std::string ANSWER = "123456780";

const char MOVE[] = {'L', 'R', 'U', 'D'};
const int64_t DELTA_X[] = {-1, 1, 0, 0};
const int64_t DELTA_Y[] = {0, 0, -1, 1};

std::string bfs_recover_path(std::string& start) {
  if (start == ANSWER) return "";

  std::unordered_set<std::string> visited;
  std::queue<Path> q;
  q.push({start, ""});

  while (!q.empty()) {
    Path element = q.front();
    q.pop();

    std::string current_state = element.state;
    std::string current_path = element.path;

    size_t free_cell = current_state.find('0');
    size_t free_y = free_cell / FIELD_WIDTH;
    size_t free_x = free_cell % FIELD_WIDTH;

    for (size_t delta = 0; delta < 4; delta++) {
      size_t new_x = DELTA_X[delta] + free_x;
      size_t new_y = DELTA_Y[delta] + free_y;

      if (new_y < 0 || new_y >= FIELD_WIDTH) continue;

      if (new_x < 0 || new_x >= FIELD_WIDTH) continue;

      size_t next_cell = new_y * FIELD_WIDTH + new_x;

      std::string next_state = current_state;
      std::swap(next_state[free_cell], next_state[next_cell]);

      Path next = {next_state, current_path + MOVE[delta]};

      if (next_state == ANSWER)
        return next.path;

      if (visited.find(next_state) == visited.end()) {
        visited.insert(next_state);
        q.push(next);
      }
    }
  }

  return NONE;
}

int main() {
  std::string start;

  for (size_t i = 0; i < FIELD_SIZE; i++) {
    int a = 0;

    std::cin >> a;

    start += '0' + a;
  }

  std::string ans = bfs_recover_path(start);

  if (ans == NONE)
  {
    std::cout << -1 << std::endl;
    return 0;
  }

  std::cout << ans.length() << std::endl;
  std::cout << ans << std::endl;

  return 0;
}
