#include <algorithm>
#include <cassert>
#include <cmath>
#include <deque>
#include <fstream>
#include <iostream>
#include <vector>

enum Color { NotVisited, InProgress, Visited };

class StrongComponentSearcher
{
public:
  StrongComponentSearcher(const size_t vertexes_amt);

  void get_edges(const size_t edges_amt);
  void find_strong_components();

  std::vector<int64_t> get_components() const { return components_; }
  int64_t              get_components_amt() const { return curr_component_ - 1; }
private:
  std::vector<std::vector<int64_t>> graph_;
  std::vector<std::vector<int64_t>> inverse_graph_;
  std::vector<Color> colors_;
  std::vector<Color> inverse_colors_;
  std::vector<int64_t> components_;
  int64_t curr_component_ = 1;

  void dfs_recursive(const int64_t start, std::vector<int64_t>& order);
  void dfs_inverse_fill_component(const int64_t start);
};

void StrongComponentSearcher::dfs_recursive(const int64_t start, std::vector<int64_t>& order) {
  colors_[start] = InProgress;
  for (size_t neighbour : graph_[start]) {
    if (colors_[neighbour] == NotVisited)
      dfs_recursive(neighbour, order);
  }
  colors_[start] = Visited;
  order.push_back(start);
}

void StrongComponentSearcher::dfs_inverse_fill_component(const int64_t start) {
  inverse_colors_[start] = InProgress;
  for (size_t neighbour : inverse_graph_[start]) {
    if (inverse_colors_[neighbour] == NotVisited)
      dfs_inverse_fill_component(neighbour);
  }
  inverse_colors_[start] = Visited;
  components_[start] = curr_component_;
}

StrongComponentSearcher::StrongComponentSearcher(const size_t vertexes_amt) :
  colors_(vertexes_amt, NotVisited), inverse_colors_(vertexes_amt, NotVisited),
  components_(vertexes_amt), graph_(vertexes_amt), inverse_graph_(vertexes_amt)
{
}

void StrongComponentSearcher::get_edges(const size_t edges_amt) {
  for (size_t i = 0; i < edges_amt; i++) {
    int64_t start = 0, end = 0;
    std::cin >> start >> end;
    start--; end--;
    graph_[start].push_back(end);
    inverse_graph_[end].push_back(start);
  }
}

void StrongComponentSearcher::find_strong_components() {
  size_t vertexes_amt = graph_.size();
  std::vector<int64_t> order;

  for (size_t i = 0; i < vertexes_amt; i++) {
    if (colors_[i] == NotVisited) {
      dfs_recursive(i, order);
    }
  }

  curr_component_ = 1;

  for (int64_t i = vertexes_amt - 1; i >= 0; i--) {
    int64_t elem = order[i];

    if (inverse_colors_[elem] == NotVisited) {
      dfs_inverse_fill_component(elem);
      curr_component_++;
    }
  }
}

int main() {
  size_t vertexes_amt = 0, edges_amt = 0;

  std::cin >> vertexes_amt >> edges_amt;

  StrongComponentSearcher searcher(vertexes_amt);
  searcher.get_edges(edges_amt);
  searcher.find_strong_components();

  int64_t components_amt = searcher.get_components_amt();
  std::vector<int64_t> components = searcher.get_components();

  std::cout << components_amt << std::endl;

  for (int64_t elem : components) {
    std::cout << elem << " ";
  }
  std::cout << std::endl;

  return 0;
}
