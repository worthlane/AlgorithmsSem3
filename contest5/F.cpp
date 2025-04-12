#include <algorithm>
#include <cassert>
#include <cmath>
#include <deque>
#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <vector>

class Graph
{
public:
  Graph(size_t size) : graph_(size, std::vector<bool>(size, false)), size_(size) {}
  Graph(const Graph& other) : graph_(other.graph_), size_(other.size_) {}

  friend std::istream& operator>>(std::istream& in, Graph& graph) {
    for (size_t i = 0; i < graph.size_; i++) {
      for (size_t j = 0; j < graph.size_; j++) {
        bool connected = false;
        in >> connected;

        if (connected) graph.graph_[i][j] = connected;
      }
    }
    return in;
  }

  friend std::ostream& operator<<(std::ostream& out, Graph& graph) {
    for (size_t i = 0; i < graph.size_; i++) {

      for (size_t j = 0; j < graph.size_; j++) {
        out << graph.graph_[i][j] << " ";
      }

      out << std::endl;
    }

    return out;
  }

  size_t get_size() const { return size_; }

  std::vector<bool>& operator[](const size_t index) { return graph_[index]; }

private:
  std::vector<std::vector<bool>> graph_;
  size_t size_;
};

Graph get_transitive_closure(const Graph& graph)
{
  Graph transitive_closure(graph);

  size_t size = graph.get_size();

  for (size_t k = 0; k < size; k++) {
    for (size_t i = 0; i < size; i++) {
      for (size_t j = 0; j < size; j++) {
        transitive_closure[i][j] = transitive_closure[i][j] || (transitive_closure[i][k] && transitive_closure[k][j]);
      }
    }
  }

  return transitive_closure;
}

int main() {
  int rooms_amount = 0;
  std::cin >> rooms_amount;

  Graph graph(rooms_amount);

  std::cin >> graph;

  Graph res = get_transitive_closure(graph);

  std::cout << res;

  return 0;
}
