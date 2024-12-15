#include <algorithm>
#include <cassert>
#include <cmath>
#include <deque>
#include <fstream>
#include <iostream>
#include <vector>
#include <cstdarg>


struct Edge {
  int64_t start;
  int64_t end;
  int64_t weight;
};

static const int64_t NO_CONNECTION = -1;

bool edge_comp(const Edge& self, const Edge& other) { return self.weight > other.weight; }

class DSU {
 public:
  DSU(const size_t cnt);

  int64_t find_set(const int64_t self_id);

  void unite_sets(const size_t amt, ...);
  void unite_two_sets(const int64_t self_id, const int64_t other_id);

 private:
  std::vector<int64_t> parent_;
  std::vector<int64_t> size_;


};

std::vector<Edge> get_edges(const size_t cnt, bool have_weights = true);

void connect_vertexes(const std::vector<Edge>& edges, std::vector<Edge>& permanent_connection, const size_t v_amount) {
  DSU dsu(v_amount);

  for (auto& edge : edges) {
    if (dsu.find_set(edge.start) != dsu.find_set(edge.end)) {
      dsu.unite_sets(2, edge.start, edge.end);

      for (auto& connection : permanent_connection) {
        if (dsu.find_set(connection.start) == dsu.find_set(connection.end) &&
            connection.weight == NO_CONNECTION) {
          connection.weight = edge.weight;
        }
      }
    }
  }
}

int main() {
  int64_t N = 0, M = 0, K = 0;

  std::cin >> N >> M >> K;

  std::vector<Edge> edges = get_edges(M);
  std::vector<Edge> permanent_connection = get_edges(K, false);

  std::sort(edges.begin(), edges.end(), edge_comp);
  connect_vertexes(edges, permanent_connection, N);

  for (auto& connection : permanent_connection) {
    std::cout << connection.weight << std::endl;
  }

  return 0;
}

std::vector<Edge> get_edges(const size_t cnt, bool have_weights) {
  std::vector<Edge> edges(cnt);

  for (size_t i = 0; i < cnt; i++) {
    int64_t start = 0, end = 0, weight = NO_CONNECTION;

    std::cin >> start >> end;

    if (have_weights) std::cin >> weight;

    edges[i].start = start - 1;
    edges[i].end = end - 1;
    edges[i].weight = weight;
  }

  return edges;
}

DSU::DSU(const size_t cnt) : parent_(cnt), size_(cnt, 1) {
  for (size_t id = 0; id < cnt; id++) {
    parent_[id] = id;
  }
}

int64_t DSU::find_set(const int64_t self_id) {
  if (self_id == parent_[self_id])
    return self_id;
  else
    return parent_[self_id] = find_set(parent_[self_id]);
}

void DSU::unite_two_sets(const int64_t self_id, const int64_t other_id) {
  int64_t self_root  = find_set(self_id);
  int64_t other_root = find_set(other_id);

  if (self_root != other_root) {
    if (size_[self_root] < size_[other_root]) std::swap(self_root, other_root);

    parent_[other_root] = self_root;
    size_[self_root] += size_[other_root];
  }
}

void DSU::unite_sets(const size_t amt, ...) {
  va_list args;
  va_start(args, amt);

  int64_t self_id = va_arg(args, int64_t);

  for (size_t i = 1; i < amt; i++) {
    int64_t other_id = va_arg(args, int64_t);

    unite_two_sets(self_id, other_id);
  }

  va_end(args);
}
