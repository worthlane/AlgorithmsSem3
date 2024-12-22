#include <algorithm>
#include <cassert>
#include <cmath>
#include <deque>
#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <vector>

static const int64_t INF = std::numeric_limits<int64_t>::max();

template <typename target>
using MinQueue = std::priority_queue<target, std::vector<target>, std::greater<target>>;

using Node = size_t;

struct Edge {
  int64_t to;
  int64_t weight;
};

struct Vertex {
  int64_t id;
  int64_t time;

  bool operator>(const Vertex& other) const { return time > other.time; }
};

class Graph
{
public:
  Graph(const size_t vertexes_amt, const size_t edges_amt) : vertexes_amt_(vertexes_amt), edges_amt_(edges_amt), graph_(vertexes_amt) {}

  friend std::istream& operator>>(std::istream& in, Graph& graph) {
    for (size_t i = 0; i < graph.edges_amt_; i++) {
      int64_t start = 0, end = 0, weight = 0;
      std::cin >> start >> end >> weight;

      start--;
      end--;

      graph[start].push_back({end, weight});
      graph[end].push_back({start, weight});
    }
    return in;
  }

size_t get_vertexes_amt() const { return vertexes_amt_; }
size_t get_edges_amt() const { return edges_amt_; }

std::vector<Edge>& operator[](const size_t index) { return graph_[index]; }
const std::vector<Edge>& operator[](const size_t index) const { return graph_[index]; }

private:
  std::vector<std::vector<Edge>> graph_;
  size_t vertexes_amt_;
  size_t edges_amt_;
};

class Solver {
public:
  Solver(const size_t viruses_amt, const size_t vertexes_amt) : viruses_amt_(viruses_amt), infected_(viruses_amt), infected_time_(vertexes_amt, INF)
  {}

  friend std::istream& operator>>(std::istream& in, Solver& solver) {
    for (int64_t i = 0; i < solver.viruses_amt_; i++) {
      Node index = 0;
      std::cin >> index;
      solver.infected_[i] = index - 1;
    }

    for (int64_t i = 0; i < solver.viruses_amt_; i++) {
      solver.infected_time_[solver.infected_[i]] = 0;
    }

    return in;
  }

  void count_infect_time(const Graph& graph);
  std::vector<int64_t> find_distances(const Graph& graph, const int64_t start);

private:
  size_t viruses_amt_;
  std::vector<Node> infected_;
  std::vector<uint64_t> infected_time_;
};
void Solver::count_infect_time(const Graph& graph) {
  MinQueue<Vertex> queue;

  for (auto& v : infected_) {
    queue.push({static_cast<int64_t>(v), 0});
  }

  while (!queue.empty()) {
    Vertex curr_pos = queue.top();
    queue.pop();

    int64_t curr_time = curr_pos.time;
    int64_t curr_id = curr_pos.id;

    if (curr_time > infected_time_[curr_id]) continue;

    for (auto& edge : graph[curr_id]) {
      int64_t to = edge.to;
      int64_t weight = edge.weight;

      int64_t time_after_pass = curr_time + weight;

      if (time_after_pass < infected_time_[to]) {
        queue.push({to, time_after_pass});
        infected_time_[to] = time_after_pass;
      }
    }
  }
}

std::vector<int64_t> Solver::find_distances(const Graph& graph, const int64_t start) {
  // опирался на https://neerc.ifmo.ru/wiki/index.php?title=%D0%90%D0%BB%D0%B3%D0%BE%D1%80%D0%B8%D1%82%D0%BC_%D0%94%D0%B5%D0%B9%D0%BA%D1%81%D1%82%D1%80%D1%8B
  int64_t N = graph.get_vertexes_amt();

  std::vector<int64_t> dist(N, INF);
  dist[start] = 0;

  MinQueue<Vertex> queue;
  queue.push({start, 0});

  while (!queue.empty()) {
    Vertex curr_pos = queue.top();
    queue.pop();

    int64_t curr_time = curr_pos.time;
    int64_t curr_id = curr_pos.id;

    if (curr_time > dist[curr_id]) continue;

    for (const auto& edge : graph[curr_id]) {
      int64_t to = edge.to;
      int64_t weight = edge.weight;

      int64_t time_after_pass = curr_time + weight;

      if (time_after_pass < infected_time_[to] && time_after_pass < dist[to]) {
        queue.push({to, time_after_pass});
        dist[to] = time_after_pass;
      }
    }
  }

  return dist;
}

int main() {
  int64_t vertexes_amt = 0, edges_amt = 0, viruses_amt = 0;

  std::cin >> vertexes_amt >> edges_amt >> viruses_amt;

  Solver solver(viruses_amt, vertexes_amt);
  std::cin >> solver;

  Graph graph(vertexes_amt, edges_amt);
  std::cin >> graph;

  int64_t start = 0, target = 0;
  std::cin >> start >> target;
  start--;
  target--;

  solver.count_infect_time(graph);

  std::vector<int64_t> dist = solver.find_distances(graph, start);

  if (dist[target] == INF)
    std::cout << "-1\n";
  else
    std::cout << dist[target] << std::endl;

  return 0;
}
