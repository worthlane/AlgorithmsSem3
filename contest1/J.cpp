#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

#define ll long long

static const ll MODULE = 1000003;
static const ll INF = 9223372036854775800;
static const size_t UNKNOWN_SIZE = -1;

std::vector<std::vector<std::pair<size_t, size_t>>> lcs_table(
    std::string& sequence1, std::string& sequence2);
std::pair<std::vector<size_t>, std::vector<size_t>> get_lcs(
    std::vector<std::vector<std::pair<size_t, size_t>>>& prev,
    std::string& sequence1, std::string& sequence2);

int main() {
  std::string sequence1 = {};
  std::string sequence2 = {};

  std::cin >> sequence1;
  std::cin >> sequence2;

  std::vector<std::vector<std::pair<size_t, size_t>>> prev =
      lcs_table(sequence1, sequence2);
  std::pair<std::vector<size_t>, std::vector<size_t>> lcs =
      get_lcs(prev, sequence1, sequence2);

  std::cout << lcs.first.size() << std::endl;

  for (size_t i = 0; i < lcs.first.size(); i++) {
    std::cout << lcs.first[i] << ' ';
  }
  std::cout << std::endl;
  for (size_t i = 0; i < lcs.first.size(); i++) {
    std::cout << lcs.second[i] << ' ';
  }
  std::cout << std::endl;

  return 0;
}

std::vector<std::vector<std::pair<size_t, size_t>>> lcs_table(
    std::string& sequence1, std::string& sequence2) {
  size_t size1 = sequence1.size();
  size_t size2 = sequence2.size();

  std::vector<std::vector<size_t>> lcs(size1 + 1,
                                       std::vector<size_t>(size2 + 1, 0));
  std::vector<std::vector<std::pair<size_t, size_t>>> prev(
      size1 + 1, std::vector<std::pair<size_t, size_t>>(
                     size2 + 1, std::make_pair(UNKNOWN_SIZE, UNKNOWN_SIZE)));

  for (size_t i = 1; i <= size1; i++) {
    for (size_t j = 1; j <= size2; j++) {
      if (sequence1[i - 1] == sequence2[j - 1]) {
        lcs[i][j] = lcs[i - 1][j - 1] + 1;
        prev[i][j] = std::pair<size_t, size_t>(i - 1, j - 1);
      } else if (lcs[i - 1][j] > lcs[i][j - 1]) {
        lcs[i][j] = lcs[i - 1][j];
        prev[i][j] = std::pair<size_t, size_t>(i - 1, j);
      } else {
        lcs[i][j] = lcs[i][j - 1];
        prev[i][j] = std::pair<size_t, size_t>(i, j - 1);
      }
    }
  }

  return prev;
}

std::pair<std::vector<size_t>, std::vector<size_t>> get_lcs(
    std::vector<std::vector<std::pair<size_t, size_t>>>& prev,
    std::string& sequence1, std::string& sequence2) {
  std::vector<size_t> lcs1 = {};
  std::vector<size_t> lcs2 = {};

  size_t i = sequence1.size();
  size_t j = sequence2.size();

  while (i != 0 && j != 0) {
    if (prev[i][j] == std::pair<size_t, size_t>(i - 1, j - 1)) {
      lcs1.push_back(i);
      lcs2.push_back(j);
      i--;
      j--;
    } else if (prev[i][j] == std::pair<size_t, size_t>(i - 1, j))
      i--;
    else
      j--;
  }

  std::reverse(lcs1.begin(), lcs1.end());
  std::reverse(lcs2.begin(), lcs2.end());

  return std::pair<std::vector<size_t>, std::vector<size_t>>(lcs1, lcs2);
}
