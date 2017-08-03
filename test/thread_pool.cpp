
#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>

#include "../src/thread_pool.hpp"


double task(const double* begin, const double* end) {
  double m(*begin);
  while (begin != end) {
    m = std::max(m, *begin);
    begin += 1;
    std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(100));
  }
  return m;
}


int main(int argc, char *argv[]) {
  unsigned int n_t = std::thread::hardware_concurrency();
  std::cout << n_t << " concurrent threads are supported.\n";
  
  std::size_t n(100);
  std::vector<double> v(n);
  std::iota(v.begin(), v.end(), 0);

  thread_pool tp(8);
  const int n_job(10);
  
  std::vector<std::future<double> > fv;
  for (int i(0); i < n_job; ++i) {
    const double * b(&v[0] + (i * n) / n_job);
    const double * e(&v[0] + std::min(((i + 1) * n) / n_job, n));
    fv.push_back(tp.enqueue(task, b, e));
  }
  
  for (int i(0); i < 10; ++i) {
    fv[i].wait();
    std::cout << "max for slice " << i << " is " << fv[i].get() << std::endl;
    }
  
  return 0;
}
