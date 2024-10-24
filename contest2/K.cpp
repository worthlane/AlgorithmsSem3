#include <algorithm>
#include <cassert>
#include <cmath>
#include <deque>
#include <fstream>
#include <iostream>
#include <vector>

#define ll int64_t

static const ll INF = 1e10;

struct Point {
  ll x;
  ll y;
};

bool x_comparator(const Point& a, const Point& b) { return a.x < b.x; }

ll x_lower_bound(std::vector<Point>& points, int x) {
  auto it =
      std::lower_bound(points.begin(), points.end(), Point{x, 0}, x_comparator);
  return it - points.begin() - 1;
}

class Line {
 public:
  Line(const ll K, const ll B) : k(K), b(B) {}

  ll eval(const ll x) const { return k * x + b; }

  bool operator==(const Line& other) const {
    return k == other.k && b == other.b;
  }
  bool operator>=(const Line& other) const {
    return k == other.k && b >= other.b;
  }
  bool operator<=(const Line& other) const {
    return k == other.k && b <= other.b;
  }

  ll k, b;
};

Point cross(const Line& first, const Line& second) {
  ll k1 = first.k;
  ll b1 = first.b;

  ll k2 = second.k;
  ll b2 = second.b;

  if (k1 == k2 && b1 == b2)
    return {0, 0};
  else if (k1 == k2 && b1 != b2)
    return {INF, INF};

  ll x = (b2 - b1) / (k1 - k2);

  if (b2 - b1 < 0) x--;

  return {x, first.eval(x)};
}

class ConvexHullTrick {
 public:
  std::vector<Point> points;
  std::vector<Line> lines;

  ConvexHullTrick() {}
  ~ConvexHullTrick() {}

  void add_line(const ll b, const ll k) {
    assert(lines.size() == points.size());

    Line new_line(k, b);

    // std::cout << k << " " << b << " ----\n";

    if (!lines.empty() && !points.empty()) {
      Line prev_line = lines.back();
      Point prev_point = points.back();

      while (prev_line.eval(prev_point.x) > new_line.eval(prev_point.x) &&
             !lines.empty()) {
        if (new_line >= prev_line) return;

        lines.pop_back();
        points.pop_back();

        prev_line = lines.back();
        prev_point = points.back();
      }
    }

    if (!lines.empty()) {
      Point cross_point = cross(lines.back(), new_line);
      points.push_back(cross_point);
    } else {
      points.push_back({-INF, -INF});
    }

    lines.push_back(new_line);
  }

  ll get_min(const ll x) {
    if (points.empty()) {
      return 0;
    }

    ll index = x_lower_bound(points, x);

    assert(index < lines.size());

    return lines[index].eval(x);
  }
};

int main() {
  size_t N = 0;
  size_t K = 0;

  std::cin >> N >> K;

  std::vector<std::vector<ll>> dp(K + 1, std::vector<ll>(N + 1, INF));

  for (ll i = 0; i <= K; i++) {
    dp[i][0] = 0;
  }

  for (ll j = 1; j <= K; j++) {
    ConvexHullTrick trick;

    for (ll i = 0; i <= N; i++) {
      ll min = trick.get_min(i);

      dp[j][i] = min + i * i;

      ll p = i + 1;
      ll p2 = p * p;

      trick.add_line(dp[j - 1][p - 1] + p2, -2 * p);
    }
  }

  if (K == 0) {
    std::cout << 0 << std::endl;
    return 0;
  }

  std::cout << dp[K][N] << std::endl;
}
