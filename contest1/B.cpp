#include <cassert>
#include <iostream>
#include <vector>

#define ll unsigned long long

static const ll MODULE = 1000003;

class Matrix {
 public:
  Matrix(const size_t height, const size_t width, std::vector<ll> values)
      : m_data_(values) {
    height_ = height;
    width_ = width;
  }
  ~Matrix() { m_data_.clear(); }

  std::vector<ll> m_data_;
  size_t height_;
  size_t width_;
};

Matrix operator*(const Matrix &a, const Matrix &b);
Matrix operator^(const Matrix &a, const ll degree);

int main() {
  ll n = 0;

  std::cin >> n;

  n--;

  Matrix ans = {5, 1, {0, 0, 0, 0, 1}};
  Matrix step = {5, 5, {0, 1, 0, 0, 0,
                        0, 0, 1, 0, 0,
                        0, 0, 0, 1, 0,
                        0, 0, 0, 0, 1,
                        1, 1, 1, 1, 1}};

  ans = (step ^ n) * ans;

  printf("%d\n", ans.m_data_[4] % MODULE);

  // std::cout << ans.m_data_[4] % MODULE << std::endl;

  return 0;
}

Matrix operator*(const Matrix &a, const Matrix &b) {
  Matrix ret = {a.height_, b.width_, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

  assert(a.width_ == b.height_);

  for (int i = 0; i < ret.width_; i++) {
    for (int j = 0; j < ret.height_; j++) {
      for (int k = 0; k < a.width_; k++) {
        ret.m_data_[i + (j * ret.width_)] +=
            (a.m_data_[k + (j * a.width_)] * b.m_data_[i + (k * b.width_)]) %
            MODULE;
      }
    }
  }

  return ret;
}

Matrix operator^(const Matrix &a, const ll degree) {
  assert(a.width_ == a.height_);

  if (degree == 0) {
    Matrix ONE = {a.height_, a.width_, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

    for (size_t i = 0; i < a.width_; i++) {
      for (size_t j = 0; j < a.height_; j++) {
        if (i == j) ONE.m_data_[i + (j * a.width_)] = 1;
      }
    }

    return ONE;
  } else if (degree == 1)
    return a;

  Matrix ans = a ^ (degree / 2);
  ans = ans * ans;
  if (degree % 2 == 1) ans = ans * a;
  return ans;
}
