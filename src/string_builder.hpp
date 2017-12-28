#ifndef _STRING_BUILDER_H_
#define _STRING_BUILDER_H_

#include <sstream>
#include <string>
#include <iomanip>


class string_builder {
  std::ostringstream oss;
  
 public:
  template<typename T>
  explicit string_builder(const T& val): oss() {
    oss << val;
  }

  template<typename T>
  string_builder& operator()(const T& val) {
    oss << val;
    return *this;
  }
  
  std::string str() {
    return oss.str();
  }
  
  operator std::string() {
    return oss.str();
  }
};

#endif /* _STRING_BUILDER_H_ */
