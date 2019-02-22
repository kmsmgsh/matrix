//
// Copyright 2019 The Statslabs Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

/// @file misc.h
/// @brief Miscellaneous element-wise functions.

#ifndef SLAB_MATRIX_FNS_MISC_H_
#define SLAB_MATRIX_FNS_MISC_H_

namespace slab {

template <typename T, std::size_t N>
inline Matrix<T, N> exp(const Matrix<T, N> &x) {
  Matrix<T, N> res = x;
  res.apply([](T &a) { a = std::exp(a); });

  return res;
}

template <typename T, std::size_t N>
inline Matrix<T, N> exp(const MatrixRef<T, N> &x) {
  Matrix<T, N> res = x;
  res.apply([](T &a) { a = std::exp(a); });

  return res;
}

template <typename T, std::size_t N>
inline Matrix<T, N> log(const Matrix<T, N> &x) {
  Matrix<T, N> res = x;
  res.apply([](T &a) { a = std::log(a); });

  return res;
}

template <typename T, std::size_t N>
inline Matrix<T, N> log(const MatrixRef<T, N> &x) {
  Matrix<T, N> res = x;
  res.apply([](T &a) { a = std::log(a); });

  return res;
}

template <typename T, typename T1, std::size_t N>
inline Matrix<T, N> pow(const Matrix<T, N> &x, const T1 &val) {
  static_assert(Convertible<T1, T>(), "pow(): incompatible element types");

  Matrix<T, N> res = x;
  res.apply([&](T &a) { a = std::pow(a, static_cast<T>(val)); });

  return res;
}

template <typename T, typename T1, std::size_t N>
inline Matrix<T, N> pow(const MatrixRef<T, N> &x, const T1 &val) {
  static_assert(Convertible<T1, T>(), "pow(): incompatible element types");

  Matrix<T, N> res = x;
  res.apply([&](T &a) { a = std::pow(a, static_cast<T>(val)); });

  return res;
}

}  // namespace slab

#endif  // SLAB_MATRIX_FNS_MISC_H_
