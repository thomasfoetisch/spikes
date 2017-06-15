#include <iostream>

template<typename D, typename ... As>
struct array_from;

template<typename D, typename A, typename ... As>
struct array_from<D, A, As...> {
  static void parameters(D* dest, const A& a, As ... as) {
    *dest = a;
    array_from<D, As...>::parameters(++dest, as...);
  }
};

template<typename D, typename A>
struct array_from<D, A> {
  static void parameters(D* dest, const A& a) {
    *dest = a;
  }
};

int main(int argc, char *argv[]) {
  int a[5];
  array_from<int, int, int, int, int, int>::parameters(a, 10, 223, 35, 489, 58);

  for(unsigned int i(0); i < 5; ++i)
    std::cout << a[i] << std::endl;

  return 0;
}
