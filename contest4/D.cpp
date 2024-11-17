#include <algorithm>
#include <cassert>
#include <cmath>
#include <deque>
#include <fstream>
#include <iostream>
#include <vector>

#define ll long long

struct Edge {
  ll start;
  ll end;
  ll weight;
};

static const ll NO_CONNECTION = -1;

bool edge_comp(const Edge& a, const Edge& b) { return a.weight > b.weight; }

class DSU {
 public:
  DSU(const size_t cnt);

  ll find_set(const ll a_id);

  void unite_sets(const ll a_id, const ll b_id);

 private:
  std::vector<ll> parent_;
  std::vector<ll> size_;
};

int main() {
  ll N = 0, M = 0, K = 0;

  std::cin >> N >> M >> K;

  std::vector<Edge> edges(M);
  std::vector<Edge> permanent_connection(K);

  for (size_t i = 0; i < M; i++) {
    ll start = 0, end = 0, weight = 0;

    std::cin >> start >> end >> weight;

    edges[i].start = start - 1;
    edges[i].end = end - 1;
    edges[i].weight = weight;
  }

  for (size_t i = 0; i < K; i++) {
    ll start = 0, end = 0;

    std::cin >> start >> end;

    permanent_connection[i].start = start - 1;
    permanent_connection[i].end = end - 1;
    permanent_connection[i].weight = NO_CONNECTION;
  }

  std::sort(edges.begin(), edges.end(), edge_comp);

  DSU dsu(N);

  for (auto& edge : edges) {
    if (dsu.find_set(edge.start) != dsu.find_set(edge.end)) {
      dsu.unite_sets(edge.start, edge.end);

      for (auto& connection : permanent_connection) {
        if (dsu.find_set(connection.start) == dsu.find_set(connection.end) &&
            connection.weight == NO_CONNECTION) {
          connection.weight = edge.weight;
        }
      }
    }
  }

  for (auto& connection : permanent_connection) {
    std::cout << connection.weight << std::endl;
  }

  return 0;
}

DSU::DSU(const size_t cnt) : parent_(cnt), size_(cnt, 1) {
  for (size_t id = 0; id < cnt; id++) {
    parent_[id] = id;
  }
}

ll DSU::find_set(const ll a_id) {
  if (a_id == parent_[a_id])
    return a_id;
  else
    return parent_[a_id] = find_set(parent_[a_id]);
}

void DSU::unite_sets(const ll a_id, const ll b_id) {
  ll a_root = find_set(a_id);
  ll b_root = find_set(b_id);

  if (a_root != b_root) {
    if (size_[a_root] < size_[b_root]) std::swap(a_root, b_root);

    parent_[b_root] = a_root;
    size_[a_root] += size_[b_root];
  }
}
