#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

#define ll long long

static const ll MODULE = 1000003;
static const ll INF = 9223372036854775800;
static const size_t UNKNOWN_SIZE = -1;

struct Elements
{
  size_t i;
  size_t j;
};

void print_vector(std::vector<size_t>& vec);

std::vector<std::vector<Elements>> lcs_table(
    std::string& sequence1, std::string& sequence2);
std::pair<std::vector<size_t>, std::vector<size_t>> get_lcs(
    std::vector<std::vector<Elements>>& prev,
    std::string& sequence1, std::string& sequence2);

int main() {
  std::string sequence1;
  std::string sequence2;

  std::cin >> sequence1;
  std::cin >> sequence2;

  std::vector<std::vector<Elements>> prev = lcs_table(sequence1, sequence2);
  std::pair<std::vector<size_t>, std::vector<size_t>> lcs = get_lcs(prev, sequence1, sequence2);

  std::cout << lcs.first.size() << std::endl;

  print_vector(lcs.first);
  print_vector(lcs.second);

  return 0;
}

std::vector<std::vector<Elements>> lcs_table(
    std::string& sequence1, std::string& sequence2) {
  size_t size1 = sequence1.size();
  size_t size2 = sequence2.size();

  std::vector<std::vector<size_t>> lcs(size1 + 1,
                                       std::vector<size_t>(size2 + 1, 0));
  std::vector<std::vector<Elements>> prev(
      size1 + 1, std::vector<Elements>(
                     size2 + 1, {UNKNOWN_SIZE, UNKNOWN_SIZE}));

  for (size_t i = 1; i <= size1; i++) {
    for (size_t j = 1; j <= size2; j++) {
      if (sequence1[i - 1] == sequence2[j - 1]) {
        lcs[i][j] = lcs[i - 1][j - 1] + 1;
        prev[i][j] = {i - 1, j - 1};
      } else if (lcs[i - 1][j] > lcs[i][j - 1]) {
        lcs[i][j] = lcs[i - 1][j];
        prev[i][j] = {i - 1, j};
      } else {
        lcs[i][j] = lcs[i][j - 1];
        prev[i][j] = {i, j - 1};
      }
    }
  }

  return prev;
}

void print_vector(std::vector<size_t>& vec)
{
  size_t size = vec.size();

  for (size_t i = 0; i < size; i++) {
    std::cout << vec[i] << ' ';
  }

  std::cout << std::endl;
}

std::pair<std::vector<size_t>, std::vector<size_t>> get_lcs(
    std::vector<std::vector<Elements>>& prev,
    std::string& sequence1, std::string& sequence2) {
  std::vector<size_t> lcs1;
  std::vector<size_t> lcs2;

  size_t i = sequence1.size();
  size_t j = sequence2.size();

  while (i != 0 && j != 0) {
    if (prev[i][j].i == i - 1 && prev[i][j].j == j - 1) {
      lcs1.push_back(i);
      lcs2.push_back(j);
      i--;
      j--;
    } else if (prev[i][j].i == i - 1 && prev[i][j].j == j)
      i--;
    else
      j--;
  }

  std::reverse(lcs1.begin(), lcs1.end());
  std::reverse(lcs2.begin(), lcs2.end());

  return std::pair<std::vector<size_t>, std::vector<size_t>>(lcs1, lcs2);
}
