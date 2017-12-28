#ifndef ANSI_IOMANIP_H
#define ANSI_IOMANIP_H

#include <iostream>
#include <algorithm>

namespace ansi {

  inline
  std::ostream& clear_line(std::ostream& stream) {
    stream << "\e[G" <<  "\e[K";
    return stream;
  }

  
  class color216 {
  public:
    color216(unsigned int r, unsigned int g, unsigned int b):
      r(std::min(r, 5u)),
      g(std::min(g, 5u)),
      b(std::min(b, 5u)) {}
    
    friend std::ostream& operator<<(std::ostream& stream, const color216& manip) {
      stream << "\e[38;5;" << 16 + 36 * manip.r + 6 * manip.g + manip.b << "m";
    }

    const unsigned int r, g, b;
  };

  
  class color {
  public:
    explicit color(unsigned int c): c(c) {}

    friend std::ostream& operator<<(std::ostream& stream, const color& manip) {
      stream << "\e[38;5;" << manip.c << "m";
    }
    
    const unsigned int c;
  };

  inline
  std::ostream& bold(std::ostream& stream) {
    stream << "\e[" << 1 << "m";
    return stream;
  }

  inline
  std::ostream& normal(std::ostream& stream) {
    stream << "\e[" << 0 << "m";
    return stream;
  }
}

#endif /* ANSI_IOMANIP_H */
