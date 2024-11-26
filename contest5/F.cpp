#include <algorithm>
#include <cassert>
#include <cmath>
#include <deque>
#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <vector>

static const int INF = 1e9;

int main() {
  int N = 0;

  std::cin >> N;

  std::vector<std::vector<bool>> graph(N, std::vector<bool>(N, false));
  for (size_t i = 0; i < N; i++) {
    for (size_t j = 0; j < N; j++) {
      bool connected = false;
      std::cin >> connected;

      if (connected) graph[i][j] = true;
    }
  }

  for (size_t k = 0; k < N; k++) {
    for (size_t i = 0; i < N; i++) {
      for (size_t j = 0; j < N; j++) {
        graph[i][j] = graph[i][j] || (graph[i][k] && graph[k][j]);
      }
    }
  }

  for (size_t i = 0; i < N; i++) {
    for (size_t j = 0; j < N; j++) {
      std::cout << graph[i][j] << " ";
    }
    std::cout << std::endl;
  }

  return 0;
}
