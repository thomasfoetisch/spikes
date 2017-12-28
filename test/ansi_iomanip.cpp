
#include <unistd.h>

#include "ansi_iomanip.hpp"

void counter(unsigned int n) {
  const unsigned int width(20);
  
  unsigned int i(0);
  while (i <= n) {

    const unsigned int fraction(std::min(n, std::max(1u, width * i / n)));
    
    std::cout << ansi::clear_line
              << '[' << std::string(fraction - 1, '=');

    if (i != n) {
      std::cout << '>' << std::string(width - fraction, ' ') << "] ";
    } else {
      std::cout << "] ";
    }
    
    std::cout << ansi::bold << ansi::color(124)
              << '(' << 100 * i / n << "%)"
              << ansi::normal
              << std::flush;

    ++i;
    usleep(100000);
  }
}

int main(int argc, char** argv) {
  counter(20);
  return 0;
  std::cout << "hello" << std::flush;
  usleep(1000000);

  std::cout << ansi::clear_line;

  std::cout << ansi::color(39);
  std::cout << ansi::bold;
  std::cout << "world" << std::endl;
  std::cout << ansi::normal;

  std::cout << "yeee" << std::endl;;
  return 0;
}
