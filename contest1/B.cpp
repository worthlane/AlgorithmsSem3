#include <cassert>
#include <iostream>
#include <vector>

#define ll unsigned long long

static const ll MODULE = 1000003;

template <typename T>
class Matrix {
 public:
  Matrix(std::vector<std::vector<T>> values) : data_(values)
  {
    height_ = values.size();
    width_  = values[0].size();
  }

  Matrix(const size_t height, const size_t width) :
        data_(height, std::vector<T>(width, 0)), height_(height), width_(width)
  {}

  ~Matrix() { data_.clear(); }

  Matrix<T> operator*(const Matrix<T> &b);
  Matrix<T> operator^(const T degree);
  Matrix<T> operator=(const Matrix<T> &other);
  std::vector<T>& operator[](const size_t index);

  std::vector<std::vector<T>> get_data()   const { return data_; }
  size_t                      get_height() const { return height_; }
  size_t                      get_width()  const { return width_; }

  private:
    std::vector<std::vector<T>> data_;
    size_t height_;
    size_t width_;
};

int main() {
  ll n = 0;

  std::cin >> n;

  n--;

  Matrix<ll> ans  = {{{0}, {0}, {0}, {0}, {1}}};
  Matrix<ll> step = {{{0, 1, 0, 0, 0},
                    {0, 0, 1, 0, 0},
                    {0, 0, 0, 1, 0},
                    {0, 0, 0, 0, 1},
                    {1, 1, 1, 1, 1}}};

  ans = (step ^ n) * ans;

  std::cout << ans[4][0] % MODULE << std::endl;
  
  return 0;
}

template <typename T> Matrix<T> Matrix<T>::operator*(const Matrix &b) {
  Matrix result = {this->get_height(), b.get_width()};

  assert(this->get_width() == b.get_height());

  for (int i = 0; i < result[0].size(); i++) {
    for (int j = 0; j < result.get_data().size(); j++) {
      for (int k = 0; k < (*this)[0].size(); k++) {
        result[j][i] += ((*this)[j][k] * (b.get_data())[k][i]) % MODULE;
      }
    }
  }

  return result;
}

template <typename T> Matrix<T> Matrix<T>::operator^(const T degree) {

  assert(this->get_width() == this->get_height());

  if (degree == 0) {

    Matrix one = {this->get_height(), this->get_width()};

    for (size_t i = 0; i < this->get_width(); i++) {
      for (size_t j = 0; j < this->get_height(); j++) {
        if (i == j) one[j][i] = 1;
      }
    }

    return one;
  } else if (degree == 1)
      return *this;

  Matrix ans = *this ^ (degree / 2);
  ans = ans * ans;

  if (degree % 2 == 1)
    ans = ans * *this;

  return ans;
}

template <typename T> Matrix<T> Matrix<T>::operator=(const Matrix<T> &other)
{
  this->data_ = other.get_data();
  this->height_ = other.get_height();
  this->width_ = other.get_width();

  return *this;
}
template <typename T> std::vector<T>& Matrix<T>::operator[](const size_t index)
{
  return this->data_[index];
}
