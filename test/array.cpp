
#include "../src/array.hpp"

void test_index() {
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
