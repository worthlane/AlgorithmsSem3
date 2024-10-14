#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

#define ll long long

static const int NO_PREV = -1;

std::vector<ll> find_hoods(const std::vector<ll>& gangsters);
void fill_dynamic_arrays(const std::vector<ll>& gangsters,
                         std::vector<ll>& greater, std::vector<ll>& less,
                         std::vector<ll>& greater_prev,
                         std::vector<ll>& less_prev);

void read_data(std::vector<ll>& gangsters);
void print_data(const std::vector<ll>& hoods);

struct EndParams
{
  size_t max_len;
  bool is_greater_zigzag;
  size_t last_elem;
};

std::vector<ll> recover_answer(const std::vector<ll>& gangsters,
                               const std::vector<ll>& greater_prev,
                               const std::vector<ll>& less_prev, const size_t max_len,
                               bool is_greater_zigzag, size_t last_elem);

EndParams find_longest_sequence(const std::vector<ll>& greater, const std::vector<ll>& less, const size_t N);

int main() {
  size_t N = 0;

  std::cin >> N;

  std::vector<ll> gangsters(N, 0);

  read_data(gangsters);

  std::vector<ll> hoods = find_hoods(gangsters);

  print_data(hoods);
}

std::vector<ll> find_hoods(const std::vector<ll>& gangsters) {
  size_t N = gangsters.size();

  std::vector<ll> greater(N, 1);  // longest zig-zag sequence with i elements
                                  // where last element is greater than previous
  std::vector<ll> less(N, 1);  // longest zig-zag sequence with i elements where
                               // last element is less than previous

  std::vector<ll> greater_prev(N, NO_PREV);
  std::vector<ll> less_prev(N, NO_PREV);

  fill_dynamic_arrays(gangsters, greater, less, greater_prev, less_prev);

  EndParams end_params = find_longest_sequence(greater, less, N);

  size_t max_len = end_params.max_len;
  bool is_greater_zigzag = end_params.is_greater_zigzag;
  size_t last_elem = end_params.last_elem;

  find_longest_sequence(greater, less, N);

  std::vector<ll> hoods = recover_answer(gangsters, greater_prev, less_prev,
                                         max_len, is_greater_zigzag, last_elem);

  return hoods;
}

void fill_dynamic_arrays(const std::vector<ll>& gangsters,
                         std::vector<ll>& greater, std::vector<ll>& less,
                         std::vector<ll>& greater_prev,
                         std::vector<ll>& less_prev) {
  size_t N = greater.size();

  for (size_t i = 0; i < N; i++) {
    for (size_t j = 0; j < i; j++) {
      if (gangsters[i] > gangsters[j] && greater[i] < less[j] + 1) {
        greater[i] = less[j] + 1;
        greater_prev[i] = j;
      } else if (gangsters[i] < gangsters[j] && less[i] < greater[j] + 1) {
        less[i] = std::max(less[i], greater[j] + 1);
        less_prev[i] = j;
      }
    }
  }
}

void read_data(std::vector<ll>& gangsters) {
  size_t N = gangsters.size();
  for (size_t i = 0; i < N; i++) {
    std::cin >> gangsters[i];
  }
}

void print_data(const std::vector<ll>& hoods) {
  size_t size = hoods.size();

  std::cout << size << std::endl;

  for (size_t i = 0; i < size; i++) {
    std::cout << hoods[i] << " ";
  }
  std::cout << std::endl;
}

std::vector<ll> recover_answer(const std::vector<ll>& gangsters,
                               const std::vector<ll>& greater_prev,
                               const std::vector<ll>& less_prev, const size_t max_len,
                               bool is_greater_zigzag, size_t last_elem) {
  std::vector<ll> hoods(max_len, 0);

  for (int i = max_len - 1; i >= 0; i--) {
    hoods[i] = gangsters[last_elem];

    if (is_greater_zigzag) {
      last_elem = greater_prev[last_elem];
      is_greater_zigzag = false;
    } else {
      last_elem = less_prev[last_elem];
      is_greater_zigzag = true;
    }
  }

  return hoods;
}

EndParams find_longest_sequence(const std::vector<ll>& greater, const std::vector<ll>& less, const size_t N) {

  size_t max_len = 0;
  bool is_greater_zigzag = false;
  size_t last_elem = NO_PREV;

  for (size_t i = 0; i < N; i++) {
    if (greater[i] > max_len) {
      last_elem = i;
      max_len = greater[i];
      is_greater_zigzag = true;
    } else if (less[i] > max_len) {
      last_elem = i;
      max_len = less[i];
      is_greater_zigzag = false;
    }
  }

  EndParams end_params = {max_len, is_greater_zigzag, last_elem};

  return end_params;

}
