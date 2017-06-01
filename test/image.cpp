#include "../src/array.hpp"


template<typename C>
class pixel {
public:
  typedef C colorspace_type;

private:
};


template<typename T>
class image {
public:
  typedef T storage_type;

private:
  array<storage_type> storage;
};

enum class method {
  nearest, linear
};

template<typename T>
array<T> interpolate(const image<T>& img,
		     double x, double y,
		     method m = method::linear) {
  switch (m) {
  case method::nearest:
    
    break;

  case method::linear:

    break;
  }
}


int main(int argc, char *argv[]) {
  
  return 0;
}
