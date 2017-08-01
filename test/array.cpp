
#include <vector>
#include <algorithm>

#include <cassert>

#include "../src/array.hpp"

void test_index() {
  {
    std::vector<std::size_t> a(5 * 3 * 2 * 6);
    std::iota(a.begin(), a.end(), 0);

    array<std::size_t> b{5, 3, 2, 6};
    b.set_data(&a[0]);

    unsigned int n(0);
    for (unsigned int i(0); i < 5; ++i)
      for (unsigned int j(0); j < 3; ++j)
	for (unsigned int k(0); k < 2; ++k)
	  for (unsigned int m(0); m < 6; ++m) {
	    assert(b.at(i, j, k, m) == a[n]);
	    n += 1;
	  }
  }

  {
    std::vector<std::size_t> a(5 * 3 * 2);
    std::iota(a.begin(), a.end(), 0);

    array<std::size_t> b{5, 3, 2};
    b.set_data(&a[0]);

    unsigned int n(0);
    for (unsigned int i(0); i < 5; ++i)
      for (unsigned int j(0); j < 3; ++j)
	for (unsigned int k(0); k < 2; ++k) {
	  assert(b.at(i, j, k) == a[n]);
	  n += 1;
	}
  }

  {
    std::vector<std::size_t> a(5 * 3);
    std::iota(a.begin(), a.end(), 0);

    array<std::size_t> b{5, 3};
    b.set_data(&a[0]);

    unsigned int n(0);
    for (unsigned int i(0); i < 5; ++i)
      for (unsigned int j(0); j < 3; ++j) {
	assert(b.at(i, j) == a[n]);
	n += 1;
      }
  }
	
  const std::size_t rank(3);
  const std::size_t sizes[] = {5, 3, 2};
  const std::size_t indices[] = {3, 2, 1};

  std::size_t index(to_linear_index(rank, sizes, indices));
  std::cout << index << std::endl;
}

void test_construction() {
  std::size_t sizes[] = {5, 3, 2};
  array<double> a(3, sizes);
  array<double> b = {5, 3, 2};
  array<double> c(a);
}

void test_assignment() {
  std::size_t sizes[] = {5, 3, 2};
  array<double> a(3, sizes);
  array<double> b = {5, 3, 2};
  array<double> c(a);

  c = a;
  b = array<double>{2, 3, 4};
}

int main(int argc, char *argv[]) {
  test_index();
  test_construction();
  
  return 0;
}
