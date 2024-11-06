#include <cassert>
#include <iostream>
#include <vector>

#define ll unsigned long long

static const ll MODULE = 1000003;

template <typename T, T MODULE>
class Modular {
  public:
    Modular(T value) : value_(value % MODULE) {}
    Modular() : value_(0) {}

    T get() const { return value_; }

    operator T() const { return value_; }

    Modular<T, MODULE> operator=(const Modular<T, MODULE>& other) {
      value_ = other.value_ % MODULE; return *this;
    }

    bool operator==(const Modular<T, MODULE>& other) const {
      return value_ == other.value_;
    }

    Modular<T, MODULE>& operator+=(const Modular<T, MODULE>& other) {
      value_ = (value_ + other.value_) % MODULE;
      return *this;
    }

    Modular<T, MODULE>& operator-=(const Modular<T, MODULE>& other) {
      value_ = (value_ - other.value_) % MODULE;
      return *this;
    }

    Modular<T, MODULE>& operator*=(const Modular<T, MODULE>& other) {
      value_ = (value_ * other.value_) % MODULE;
      return *this;
    }

    Modular<T, MODULE>& operator/=(const Modular<T, MODULE>& other) {
      value_ = (value_ / other.value_) % MODULE;
      return *this;
    }

    Modular<T, MODULE> operator-() const {
      return Modular<T, MODULE>(-value_);
    }

    Modular<T, MODULE> operator+(const Modular<T, MODULE>& other) const {
      return Modular<T, MODULE>(value_ + other.value_);
    }

    Modular<T, MODULE> operator-(const Modular<T, MODULE>& other) const {
      return Modular<T, MODULE>(value_ - other.value_);
    }

    Modular<T, MODULE> operator*(const Modular<T, MODULE>& other) const {
      return Modular<T, MODULE>(value_ * other.value_);
    }

    Modular<T, MODULE> operator/(const Modular<T, MODULE>& other) const {
      return Modular<T, MODULE>(value_ / other.value_);
    }

    void print() { std::cout << value_ << std::endl; }

  private:
    T value_;
};

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

  Matrix(const size_t n, const T& diag_value, const T& other_value) :
    Matrix(n, n)
  {
    for (size_t i = 0; i < n; i++)
    {
      for (size_t j = 0; j < n; j++)
        data_[i][j] = other_value;

      data_[i][i] = diag_value;
    }
  }


  ~Matrix() { data_.clear(); }

  Matrix<T> operator*(const Matrix<T> &other);
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

ll get_paths_amount(Matrix<Modular<ll, MODULE>> start, Matrix<Modular<ll, MODULE>> step, const ll n) {

  Matrix<Modular<ll, MODULE>> answer  = start;

  answer = (step ^ n) * answer;

  return answer[answer.get_height() - 1][0];
}

int main() {
  ll n = 0;

  std::cin >> n;
  n--;

  Matrix<Modular<ll, MODULE>> answer  = {{{0}, {0}, {0}, {0}, {1}}};
  Matrix<Modular<ll, MODULE>> step =  {{{0, 1, 0, 0, 0},
                                        {0, 0, 1, 0, 0},
                                        {0, 0, 0, 1, 0},
                                        {0, 0, 0, 0, 1},
                                        {1, 1, 1, 1, 1}}};

  ll paths_amount = get_paths_amount(answer, step, n);

  std::cout << paths_amount << std::endl;

  return 0;
}

template <typename T> Matrix<T> Matrix<T>::operator*(const Matrix &other) {
  Matrix result = {this->get_height(), other.get_width()};

  assert(this->get_width() == other.get_height());

  for (int i = 0; i < result[0].size(); i++) {
    for (int j = 0; j < result.get_data().size(); j++) {
      for (int k = 0; k < (*this)[0].size(); k++) {
        result[j][i] += ((*this)[j][k] * (other.get_data())[k][i]);
      }
    }
  }

  return result;
}

template <typename T> Matrix<T> Matrix<T>::operator^(const T degree) {

  assert(this->get_width() == this->get_height());

  Matrix one = {this->get_height(), 1, 0};

  if (degree == T(0)) {
    return one;
  }

  if (degree == T(1))
    return *this;

  Matrix answer = *this ^ (degree / T(2));
  answer = answer * answer;

  if (degree % T(2) == T(1))
    answer = answer * *this;

  return answer;
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
