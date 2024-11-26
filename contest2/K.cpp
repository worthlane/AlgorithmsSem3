#include <algorithm>
#include <cassert>
#include <cmath>
#include <deque>
#include <fstream>
#include <iostream>
#include <vector>

static const int64_t INF = 1e10;

struct Point {
  int64_t x;
  int64_t y;
};

bool x_comparator(const Point& a, const Point& intercept) { return a.x < intercept.x; }

int64_t x_lower_bound(std::vector<Point>& points, int x) {
  auto it =
      std::lower_bound(points.begin(), points.end(), Point{x, 0}, x_comparator);
  return it - points.begin() - 1;
}

class Line {
 public:
  Line(const int64_t K, const int64_t B) : slope(K), intercept(B) {}

  int64_t eval(const int64_t x) const { return slope * x + intercept; }

  bool operator==(const Line& other) const {
    return slope == other.slope && intercept == other.intercept;
  }
  bool operator>=(const Line& other) const {
    return slope == other.slope && intercept >= other.intercept;
  }
  bool operator<=(const Line& other) const {
    return slope == other.slope && intercept <= other.intercept;
  }

  int64_t slope, intercept;
};

Point intersect(const Line& first, const Line& second) {
  int64_t k1 = first.slope;
  int64_t b1 = first.intercept;

  int64_t k2 = second.slope;
  int64_t b2 = second.intercept;

  if (k1 == k2 && b1 == b2)
    return {0, 0};
  else if (k1 == k2 && b1 != b2)
    return {INF, INF};

  int64_t x = (b2 - b1) / (k1 - k2);

  if (b2 - b1 < 0) x--;

  return {x, first.eval(x)};
}

class ConvexHullTrick {
 public:
  std::vector<Point> points;
  std::vector<Line> lines;

  ConvexHullTrick() {}
  ~ConvexHullTrick() {}

  void add_line(const int64_t slope, const int64_t intercept) {
    assert(lines.size() == points.size());

    Line new_line(slope, intercept);

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
      Point cross_point = intersect(lines.back(), new_line);
      points.push_back(cross_point);
    } else {
      points.push_back({-INF, -INF});
    }

    lines.push_back(new_line);
  }

  int64_t get_min(const int64_t x) {
    if (points.empty()) {
      return 0;
    }

    int64_t index = x_lower_bound(points, x);

    assert(index < lines.size());

    return lines[index].eval(x);
  }
};

int64_t solve(const int64_t points_amount, const int64_t segments_amount) {

  std::vector<std::vector<int64_t>> dp(segments_amount + 1, std::vector<int64_t>(points_amount + 1, INF));

  for (int64_t i = 0; i <= segments_amount; i++) {
    dp[i][0] = 0;
  }

  for (int64_t j = 1; j <= segments_amount; j++) {
    ConvexHullTrick trick;

    for (int64_t i = 0; i <= points_amount; i++) {
      int64_t min = trick.get_min(i);

      dp[j][i] = min + i * i;

      int64_t p = i + 1;
      int64_t p2 = p * p;

      trick.add_line(-2 * p, dp[j - 1][p - 1] + p2);
    }
  }

  return segments_amount == 0 ? 0 : dp[segments_amount][points_amount];
}

int main() {
  size_t points_amount = 0;
  size_t segments_amount = 0;

  std::cin >> points_amount >> segments_amount;

  std::cout << solve(points_amount, segments_amount) << std::endl;
}
