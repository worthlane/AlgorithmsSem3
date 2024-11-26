#include <algorithm>
#include <cassert>
#include <cmath>
#include <deque>
#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <vector>

static const int64_t INF = 1e19;

struct Edge {
  int64_t to;
  int64_t weight;
};

struct Vertex {
  int64_t id;
  int64_t time;

  bool operator>(const Vertex& other) const { return time > other.time; }
};

void count_infect_time(std::vector<std::vector<Edge>>& graph,
                       std::vector<int64_t>& infected,
                       std::vector<int64_t>& infected_time) {
  std::priority_queue<Vertex, std::vector<Vertex>, std::greater<Vertex>> q;

  for (auto& v : infected) {
    q.push({v, 0});
  }

  while (!q.empty()) {
    Vertex curr_pos = q.top();
    q.pop();

    int64_t curr_time = curr_pos.time;
    int64_t curr_id = curr_pos.id;

    if (curr_time > infected_time[curr_id]) continue;

    for (auto& edge : graph[curr_id]) {
      int64_t to = edge.to;
      int64_t weight = edge.weight;

      int64_t time_after_pass = curr_time + weight;

      if (time_after_pass < infected_time[to]) {
        q.push({to, time_after_pass});
        infected_time[to] = time_after_pass;
      }
    }
  }
}

std::vector<int64_t> deykstra(std::vector<std::vector<Edge>>& graph, const int64_t start,
                             std::vector<int64_t>&infected_time) {
// опирался на https://neerc.ifmo.ru/wiki/index.php?title=%D0%90%D0%BB%D0%B3%D0%BE%D1%80%D0%B8%D1%82%D0%BC_%D0%94%D0%B5%D0%B9%D0%BA%D1%81%D1%82%D1%80%D1%8B
  int64_t N = graph.size();

  std::vector<int64_t> dist(N, INF);
  dist[start] = 0;

  std::priority_queue<Vertex, std::vector<Vertex>, std::greater<Vertex>> q;
  q.push({start, 0});

  while (!q.empty()) {
    Vertex curr_pos = q.top();
    q.pop();

    int64_t curr_time = curr_pos.time;
    int64_t curr_id = curr_pos.id;

    if (curr_time > dist[curr_id]) continue;

    for (auto& edge : graph[curr_id]) {
      int64_t to = edge.to;
      int64_t weight = edge.weight;

      int64_t time_after_pass = curr_time + weight;

      if (time_after_pass < infected_time[to] && time_after_pass < dist[to]) {
        q.push({to, time_after_pass});
        dist[to] = time_after_pass;
      }
    }
  }

  return dist;
}

int main() {
  int64_t N = 0, M = 0, K = 0;

  std::cin >> N >> M >> K;

  std::vector<int64_t> infected(K);

  for (int64_t i = 0; i < K; i++) {
    int64_t v = 0;
    std::cin >> v;
    infected[i] = v - 1;
  }

  std::vector<int64_t> infected_time(N, INF);

  for (int64_t i = 0; i < K; i++) {
    infected_time[infected[i]] = 0;
  }

  std::vector<std::vector<Edge>> graph(N);

  for (size_t i = 0; i < M; i++) {
    int64_t a = 0, b = 0, w = 0;
    std::cin >> a >> b >> w;

    a--;
    b--;

    graph[a].push_back({b, w});
    graph[b].push_back({a, w});
  }

  int64_t s = 0, t = 0;
  std::cin >> s >> t;
  s--;
  t--;

  count_infect_time(graph, infected, infected_time);

  std::vector<int64_t> dist = deykstra(graph, s, infected_time);

  if (dist[t] == INF)
    std::cout << "-1\n";
  else
    std::cout << dist[t] << std::endl;

  return 0;
}
