#include <algorithm>
#include <cassert>
#include <cmath>
#include <deque>
#include <fstream>
#include <iostream>
#include <vector>

enum Color { NotVisited, InProgress, Visited };

void dfs_recursive(std::vector<std::vector<int64_t>>& graph, const int64_t start,
                   std::vector<Color>& colors, std::vector<int64_t>& order) {
  colors[start] = InProgress;
  for (size_t neighbour : graph[start]) {
    if (colors[neighbour] == NotVisited)
      dfs_recursive(graph, neighbour, colors, order);
  }
  colors[start] = Visited;
  order.push_back(start);
}

void dfs_fill_component(std::vector<std::vector<int64_t>>& graph, const int64_t start,
                        std::vector<Color>& colors, std::vector<int64_t>& components,
                        const int64_t curr_component) {
  colors[start] = InProgress;
  for (size_t neighbour : graph[start]) {
    if (colors[neighbour] == NotVisited)
      dfs_fill_component(graph, neighbour, colors, components, curr_component);
  }
  colors[start] = Visited;
  components[start] = curr_component;
}

int main() {
  size_t vertexes_amt = 0, edges_amt = 0;

  std::cin >> vertexes_amt >> edges_amt;

  std::vector<std::vector<int64_t>> graph(vertexes_amt);
  std::vector<std::vector<int64_t>> inverse_graph(vertexes_amt);

  for (size_t i = 0; i < edges_amt; i++) {
    int64_t start = 0, end = 0;
    std::cin >> start >> end;
    start--;
    end--;
    graph[start].push_back(end);
    inverse_graph[end].push_back(start);
  }

  std::vector<Color> colors(vertexes_amt, NotVisited);
  std::vector<int64_t> order;

  for (size_t i = 0; i < vertexes_amt; i++) {
    if (colors[i] == NotVisited) {
      dfs_recursive(graph, i, colors, order);
    }
  }

  std::vector<int64_t> components(vertexes_amt, 0);
  std::vector<Color> inverse_colors(vertexes_amt, NotVisited);

  int64_t curr_component = 1;

  for (int64_t i = vertexes_amt - 1; i >= 0; i--) {
    int64_t elem = order[i];

    if (inverse_colors[elem] == NotVisited) {
      dfs_fill_component(inverse_graph, elem, inverse_colors, components,
                         curr_component);
      curr_component++;
    }
  }

  std::cout << curr_component - 1 << std::endl;

  for (int64_t elem : components) {
    std::cout << elem << " ";
  }
  std::cout << std::endl;

  return 0;
}
