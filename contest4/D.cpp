#include <algorithm>
#include <cassert>
#include <cmath>
#include <deque>
#include <fstream>
#include <iostream>
#include <vector>
//#include <stdarg.h>

#define ll long long

struct Edge {
  ll start;
  ll end;
  ll weight;
};

static const ll NO_CONNECTION = -1;

bool edge_comp(const Edge& self, const Edge& other) { return self.weight > other.weight; }

class DSU {
 public:
  DSU(const size_t cnt);

  ll find_set(const ll self_id);

  void unite_sets(const size_t amt, ...);
  void unite_two_sets(const ll self_id, const ll other_id);

 private:
  std::vector<ll> parent_;
  std::vector<ll> size_;


};

std::vector<Edge> get_edges(const size_t cnt, bool have_weights = true);

void connect_vertexes(std::vector<Edge>& edges, std::vector<Edge>& permanent_connection, const size_t v_amount) {
  std::sort(edges.begin(), edges.end(), edge_comp);
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
  ll N = 0, M = 0, K = 0;

  std::cin >> N >> M >> K;

  std::vector<Edge> edges = get_edges(M);
  std::vector<Edge> permanent_connection = get_edges(K, false);

  connect_vertexes(edges, permanent_connection, N);

  for (auto& connection : permanent_connection) {
    std::cout << connection.weight << std::endl;
  }

  return 0;
}

std::vector<Edge> get_edges(const size_t cnt, bool have_weights) {
  std::vector<Edge> edges(cnt);

  for (size_t i = 0; i < cnt; i++) {
    ll start = 0, end = 0, weight = NO_CONNECTION;

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

ll DSU::find_set(const ll self_id) {
  if (self_id == parent_[self_id])
    return self_id;
  else
    return parent_[self_id] = find_set(parent_[self_id]);
}

void DSU::unite_two_sets(const ll self_id, const ll other_id) {
  ll self_root  = find_set(self_id);
  ll other_root = find_set(other_id);

  if (self_root != other_root) {
    if (size_[self_root] < size_[other_root]) std::swap(self_root, other_root);

    parent_[other_root] = self_root;
    size_[self_root] += size_[other_root];
  }
}

void DSU::unite_sets(const size_t amt, ...) {
  va_list args;
  va_start(args, amt);

  ll self_id = va_arg(args, ll);

  for (size_t i = 1; i < amt; i++) {
    ll other_id = va_arg(args, ll);

    unite_two_sets(self_id, other_id);
  }

  va_end(args);
}
