#include <algorithm>
#include <cassert>
#include <cmath>
#include <deque>
#include <fstream>
#include <iostream>
#include <vector>

static const int64_t INF = 1e10;

class BigInt {
 public:
  explicit BigInt(const std::string& s) {
    for (size_t i = 0; i < s.size(); i++) {
      digits.push_back(s[i] - '0');
    }

    std::reverse(digits.begin(), digits.end());
  }

  explicit BigInt(int n) {
    while (n > 0) {
      digits.push_back(n % 10);
      n /= 10;
    }
  }

  explicit BigInt() : BigInt(0) {}

  BigInt operator+(const BigInt& other) const {
    BigInt res;

    size_t max_len = std::max(digits.size(), other.digits.size());
    int remain = 0;

    for (size_t i = 0; (i < max_len) || remain; i++) {
      int diff = remain;

      if (i < digits.size()) diff += digits[i];

      if (i < other.digits.size()) diff += other.digits[i];

      res.digits.push_back(diff % 10);

      remain = diff / 10;
    }

    return res;
  }

  BigInt add(const BigInt& other, const int64_t mod) {
    return (*this + other) % mod;
  }

  BigInt operator-(const BigInt& other) const {
    BigInt res;

    assert(digits.size() >= other.digits.size());

    size_t max_len = std::max(digits.size(), other.digits.size());
    int remain = 0;

    for (size_t i = 0; i < max_len || remain; i++) {
      int diff = remain;

      if (i < digits.size()) diff += digits[i];

      if (i < other.digits.size()) diff -= other.digits[i];

      if (diff < 0) {
        diff += 10;
        remain = -1;
      } else {
        remain = 0;
      }

      res.digits.push_back(diff);
    }

    while (res.digits.size() > 1 && res.digits.back() == 0)
      res.digits.pop_back();

    return res;
  }

  BigInt sub(const BigInt& other, const int64_t mod) {
    return (*this - other) % mod;
  }

  BigInt operator*(const BigInt& other) const {
    BigInt res;

    res.digits.resize(digits.size() + other.digits.size());

    for (size_t i = 0; i < digits.size(); i++) {
      for (size_t j = 0; j < other.digits.size(); j++) {
        res.digits[i + j] += digits[i] * other.digits[j];
        res.digits[i + j + 1] += res.digits[i + j] / 10;
        res.digits[i + j] %= 10;
      }
    }

    while (res.digits.size() > 0 && res.digits.back() == 0)
      res.digits.pop_back();

    return res;
  }

  BigInt mul(const BigInt& other, const int64_t mod) {
    return (*this * other) % mod;
  }

  BigInt operator/(const int64_t other) const {
    BigInt res;

    int64_t remain = 0;

    for (int i = digits.size() - 1; i >= 0; i--) {
      remain = remain * 10 + digits[i];
      res.digits.push_back(remain / other);
      remain %= other;
    }

    std::reverse(res.digits.begin(), res.digits.end());

    if (res.digits.size() > 0 && res.digits.back() == 0) res.digits.pop_back();

    return res;
  }

  BigInt div(const int64_t other, const int64_t mod) { return (*this / other) % mod; }

  BigInt operator%(const int64_t other) {
    BigInt quotient(*this / other);

    return *this - (quotient * static_cast<BigInt>(other));
  }

  bool operator==(const BigInt& other) {
    if (digits.size() != other.digits.size()) return false;

    for (size_t i = 0; i < digits.size(); i++) {
      if (digits[i] != other.digits[i]) return false;
    }

    return true;
  }

  bool operator!=(const BigInt& other) { return !(*this == other); }

  std::vector<int> digits;
};

std::ostream& operator << (std::ostream& out, const BigInt& number) {
  if (number.digits.size() == 0) {
    return out << '0';
  }

  for (int i = number.digits.size() - 1; i >= 0; i--) {
    out << static_cast<char>(number.digits[i] + '0');
  }

  return out;
}

std::istream& operator >> (std::istream& in, BigInt& number) {
  std::string s;
  in >> s;
  number = BigInt(s);
  return in;
}

template <typename T>
class Matrix {
 public:
  Matrix(const size_t rows, const size_t columns)
      : rows(rows), columns(columns), data(rows, std::vector<T>(columns, 0)) {}

  std::vector<T>& operator[](size_t i) { return data[i]; }

  Matrix mul(const Matrix& other, const T mod) {
    assert(columns == other.rows);

    Matrix res(rows, other.columns);

    for (size_t i = 0; i < rows; i++) {
      for (size_t j = 0; j < other.columns; j++) {
        for (size_t k = 0; k < columns; k++) {
          res.data[i][j] =
              (res.data[i][j] + (data[i][k] * other.data[k][j]) % mod) % mod;
        }
      }
    }

    return res;
  }

  Matrix pow(BigInt& power, const T mod) {
    assert(rows == columns);

    Matrix res(rows, rows);
    for (size_t i = 0; i < rows; i++) res[i][i] = 1;

    Matrix a = *this;

    BigInt zero(0);

    while (power != zero) {
      if (power % 2 == static_cast<BigInt>(1)) res = res.mul(a, mod);

      a = a.mul(a, mod);
      power = power / 2;
    }

    return res;
  }

  size_t rows, columns;
  std::vector<std::vector<T>> data;
};

bool get_bit(size_t mask, size_t pos) { return (mask & (1 << pos)) != 0; }

bool has_transistion(size_t mask_1, size_t mask_2, size_t mask_len) {
  bool prev_bit_1 = get_bit(mask_1, 0);
  bool prev_bit_2 = get_bit(mask_2, 0);

  for (size_t i = 1; i < mask_len; i++) {
    bool cur_bit_1 = get_bit(mask_1, i);
    bool cur_bit_2 = get_bit(mask_2, i);

    if (cur_bit_1 == cur_bit_2 && prev_bit_1 == prev_bit_2 &&
        cur_bit_1 == prev_bit_1) {
      return false;
    }

    prev_bit_1 = cur_bit_1;
    prev_bit_2 = cur_bit_2;
  }
  return true;
}

Matrix<int64_t> make_transistion_matrix(const size_t mask_len) {
  size_t max_profile = (1 << mask_len) - 1;

  Matrix<int64_t> profile(max_profile + 1, max_profile + 1);

  for (size_t i = 0; i <= max_profile; i++) {
    for (size_t j = 0; j <= max_profile; j++) {
      if (has_transistion(i, j, mask_len))
        profile[i][j] = 1;
      else
        profile[i][j] = 0;
    }
  }

  return profile;
}

int main() {
  int64_t city_length = 0, mod = 0;
  BigInt  city_width;

  std::cin >> city_width >> city_length >> mod;

  size_t max_profile = (1 << city_length) - 1;

  Matrix<int64_t> profile = make_transistion_matrix(city_length);

  city_width = city_width - BigInt(1);

  Matrix<int64_t> res = profile.pow(city_width, mod);

  int64_t ans = 0;

  for (size_t i = 0; i <= max_profile; i++) {
    for (size_t j = 0; j <= max_profile; j++) {
      ans = (ans + res[i][j]) % mod;
    }
  }

  std::cout << ans << std::endl;

  return 0;
}
