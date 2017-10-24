#ifndef _VECTOR_H_
#define _VECTOR_H_


template<std::size_t m, std::size_t n>
class matrix {
 public:

  matrix() {}

  matrix(const matrix<m, n>& mat) {
    std::copy(mat.buffer, mat.buffer + n * m, buffer);
  }
  
  const double& operator()(std::size_t i, std::size_t j) const {
    return buffer[i * n + j];
  }
  
  double& operator()(std::size_t i, std::size_t j) {
    return buffer[i * n + j];
  }

 private:
  double buffer[m * n];

 private:
};



matrix<2, 2> invert(const matrix<2, 2>& m) {
  matrix<2, 2> result;
  
  const double det(m(0, 0)*m(1, 1) - m(1, 0) * m(0, 1));

  result(0, 0) = m(1, 1) / det;
  result(0, 1) = - m(0, 1) / det;
  result(1, 0) = - m(1, 0) / det;
  result(1, 1) = m(0, 0) / det;

  return result;
}

matrix<3, 3> invert(const matrix<3, 3>& m) {
  matrix<3, 3> result;
  
  const double det(m(0, 0) * m(1, 1) * m(2, 2)
                   + m(0, 1) * m(1, 2) * m(2, 0)
                   + m(0, 2) * m(1, 0) * m(2, 1)
                   - m(2, 0) * m(1, 1) * m(0, 2)
                   - m(2, 1) * m(1, 2) * m(0, 0)
                   - m(2, 2) * m(1, 0) * m(0, 1));
                   

  result(0, 0) =  (m(1, 1) * m(2, 2) - m(1, 2) * m(2, 1)) / det;
  result(0, 1) = -(m(0, 1) * m(2, 2) - m(0, 2) * m(2, 1)) / det;
  result(0, 2) =  (m(0, 1) * m(1, 2) - m(0, 2) * m(1, 1)) / det;
  result(1, 0) = -(m(1, 0) * m(2, 2) - m(1, 2) * m(2, 0)) / det;
  result(1, 1) =  (m(0, 0) * m(2, 2) - m(0, 2) * m(2, 0)) / det;
  result(1, 2) = -(m(0, 0) * m(1, 2) - m(0, 2) * m(1, 0)) / det;
  result(2, 0) =  (m(1, 0) * m(2, 1) - m(1, 1) * m(2, 0)) / det;
  result(2, 1) = -(m(0, 0) * m(2, 1) - m(0, 1) * m(2, 0)) / det;
  result(2, 2) =  (m(0, 0) * m(1, 1) - m(0, 1) * m(1, 0)) / det;
  

  return result;
}


template<std::size_t n>
matrix<n, n> invert(const matrix<n, n>& mat) {
  matrix<n, n> result(mat);

  int N(n);
  int ipiv[n];
  int lwork(n * n);
  double work[n * n];
  int info(0);

  dgetrf_(&N, &N, &result(0, 0), &N, ipiv, &info);
  dgetri_(&N, &result(0, 0), &N, ipiv, work, &lwork, &info);

  return result;
}

template<std::size_t n>
class vector {
  template<std::size_t m>
  friend class vector;
  
 public:
  vector() {}

  vector(double x) {
    for (std::size_t i(0); i < n; ++i)
      buffer[i] = x;
  }

  vector(double x_1, double x_2) {
    buffer[0] = x_1, buffer[1] = x_2;
  }
  vector(double x_1, double x_2, double x_3) {
    buffer[0] = x_1, buffer[1] = x_2, buffer[2] = x_3;
  }
  
  vector(double* v) {
    for (std::size_t i(0); i < n; ++i)
      buffer[i] = v[i];
  }

  vector(const vector<n>& a) {
    for (std::size_t i(0); i < n; ++i)
      buffer[i] = a.buffer[i];
  }
  
  template<std::size_t m>
  explicit vector(const vector<m>& a) {
    for (std::size_t i(0); i < m and i < n; ++i)
      buffer[i] = a.buffer[i];
  }
  
  const double& operator[](std::size_t i) const { return buffer[i]; }
  double& operator[](std::size_t i) { return buffer[i]; }

  bool operator==(const vector<n>& op) const {
    for (std::size_t i(0); i < n; ++i)
      if (buffer[i] != op.buffer[i])
        return false;
    return true;
  }
  
  bool operator<(const vector<n>& op) const {
    for (std::size_t i(0); i < n; ++i)
      if (buffer[i] < op.buffer[i])
        return true;
      else if (buffer[i] > op.buffer[i])
        return false;

    return false;
  }

 private:
  double buffer[n];
};

template<std::size_t n>
vector<n> operator/(const vector<n>& vec, double s) {
  vector<n> result;

  for (std::size_t i(0); i < n; ++i)
    result[i] = vec[i] / s;

  return result;
}

template<std::size_t n>
vector<n> operator*(const vector<n>& vec, double s) {
  vector<n> result;

  for (std::size_t i(0); i < n; ++i)
    result[i] = vec[i] * s;

  return result;
}

template<std::size_t n>
vector<n> operator*(double s, const vector<n>& vec) {
  return operator*(vec, s);
}


template<std::size_t n>
vector<n> operator+(const vector<n>& vec1, const vector<n>& vec2) {
  vector<n> result;

  for (std::size_t i(0); i < n; ++i)
    result[i] = vec1[i] + vec2[i];

  return result;
}

template<std::size_t n>
vector<n> operator-(const vector<n>& vec1, const vector<n>& vec2) {
  vector<n> result;

  for (std::size_t i(0); i < n; ++i)
    result[i] = vec1[i] - vec2[i];

  return result;
}


template<std::size_t n>
std::ostream& operator<<(std::ostream& stream, const vector<n>& a) {
  stream << "{ ";
  for (std::size_t i(0); i < n; ++i)
    stream << a[i] << " ";
  stream << "}";

  return stream;
}

template<std::size_t m, std::size_t n>
vector<m> operator*(const matrix<m, n>& mat, const vector<n>& vec) {
  vector<m> result;

  for (std::size_t k(0); k < m; ++k) {
    result[k] = 0.0;
    for (std::size_t i(0); i < n; ++i)
      result[k] += mat(k, i) * vec[i];
  }

  return result;
}


template<std::size_t n>
double norm_inf(const vector<n>& vec) {
  double max(std::abs(vec[0]));

  for (std::size_t i(1); i < n; ++i)
    if (max < std::abs(vec[i]))
      max = std::abs(vec[i]);

  return max;
}

template<std::size_t n>
double norm(const vector<n>& vec, int k) {
  double sum(0.0);

  for (std::size_t i(0); i < n; ++i)
    sum += std::pow(vec[i], k);

  return std::pow(sum, 1.0 / static_cast<double>(k));
}

#endif /* _VECTOR_H_ */
