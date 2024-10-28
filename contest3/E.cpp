#include <algorithm>
#include <cassert>
#include <cmath>
#include <deque>
#include <fstream>
#include <iostream>
#include <vector>

#define ll long long

enum Color { WHITE, GRAY, BLACK };

void dfs_recursive(std::vector<std::vector<ll>>& graph, const ll start,
                   std::vector<Color>& colors, std::vector<ll>& order) {
  colors[start] = GRAY;
  for (size_t neighbour : graph[start]) {
    if (colors[neighbour] == WHITE)
      dfs_recursive(graph, neighbour, colors, order);
  }
  colors[start] = BLACK;
  order.push_back(start);
}

void dfs_fill_component(std::vector<std::vector<ll>>& graph, const ll start,
                        std::vector<Color>& colors, std::vector<ll>& components,
                        const ll curr_component) {
  colors[start] = GRAY;
  for (size_t neighbour : graph[start]) {
    if (colors[neighbour] == WHITE)
      dfs_fill_component(graph, neighbour, colors, components, curr_component);
  }
  colors[start] = BLACK;
  components[start] = curr_component;
}

int main() {
  size_t N = 0, M = 0;

  std::cin >> N >> M;

  std::vector<std::vector<ll>> graph(N);
  std::vector<std::vector<ll>> inverse_graph(N);

  for (size_t i = 0; i < M; i++) {
    ll a = 0, b = 0;
    std::cin >> a >> b;
    a--;
    b--;
    graph[a].push_back(b);
    inverse_graph[b].push_back(a);
  }

  std::vector<Color> colors(N, WHITE);
  std::vector<ll> order;

  for (size_t i = 0; i < N; i++) {
    if (colors[i] == WHITE) {
      dfs_recursive(graph, i, colors, order);
    }
  }

  std::vector<ll> components(N, 0);
  std::vector<Color> inverse_colors(N, WHITE);

  ll curr_component = 1;

  for (ll i = N - 1; i >= 0; i--) {
    ll elem = order[i];

    if (inverse_colors[elem] == WHITE) {
      dfs_fill_component(inverse_graph, elem, inverse_colors, components,
                         curr_component);
      curr_component++;
    }
  }

  std::cout << curr_component - 1 << std::endl;

  for (ll elem : components) {
    std::cout << elem << " ";
  }
  std::cout << std::endl;

  return 0;
}
