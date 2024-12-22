#include <algorithm>
#include <cassert>
#include <cmath>
#include <deque>
#include <fstream>
#include <iostream>
#include <vector>

enum Color { NotVisited, InProgress, Visited };

class Graph
{
public:
  Graph(const size_t vertexes_amt, const size_t edges_amt) : vertexes_amt_(vertexes_amt), edges_amt_(edges_amt), graph_(vertexes_amt) {}

  friend std::istream& operator>>(std::istream& in, Graph& graph);

  size_t get_vertexes_amt() const { return vertexes_amt_; }
  size_t get_edges_amt() const { return edges_amt_; }

  std::vector<int64_t>& operator[](const size_t index) { return graph_[index]; }
  const std::vector<int64_t>& operator[](const size_t index) const { return graph_[index]; }

  Graph get_inverse_graph() const;

private:
  std::vector<std::vector<int64_t>> graph_;
  size_t vertexes_amt_;
  size_t edges_amt_;
};

Graph Graph::get_inverse_graph() const {
  Graph inverse_graph(vertexes_amt_, edges_amt_);

  for (size_t i = 0; i < vertexes_amt_; i++) {
    for (size_t j = 0; j < graph_[i].size(); j++) {
      inverse_graph[graph_[i][j]].push_back(i);
    }
  }

  return inverse_graph;
}

std::istream& operator>>(std::istream& in, Graph& graph) {
  for (size_t i = 0; i < graph.edges_amt_; i++) {
    int64_t start = 0, end = 0;
    std::cin >> start >> end;

    start--;
    end--;

    graph.graph_[start].push_back(end);
  }
  return in;
}

class StrongComponentSearcher
{
public:
  StrongComponentSearcher(const size_t vertexes_amt, Graph& graph);

  void find_strong_components();

  std::vector<int64_t> get_components() const { return components_; }
  int64_t              get_components_amt() const { return curr_component_ - 1; }

private:
  Graph graph_;
  Graph inverse_graph_;
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

StrongComponentSearcher::StrongComponentSearcher(const size_t vertexes_amt, Graph& graph) :
  colors_(vertexes_amt, NotVisited), inverse_colors_(vertexes_amt, NotVisited),
  components_(vertexes_amt), graph_(graph), inverse_graph_(graph.get_inverse_graph())
{}

void StrongComponentSearcher::find_strong_components() {
  size_t vertexes_amt = graph_.get_vertexes_amt();
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

  Graph graph(vertexes_amt, edges_amt);
  std::cin >> graph;

  StrongComponentSearcher searcher(vertexes_amt, graph);
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
