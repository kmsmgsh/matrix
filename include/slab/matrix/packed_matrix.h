//
// Copyright 2018 The Statslabs Authors.
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

/// @file packed_matrix.h
/// @brief Packed matrix

#ifndef SLAB_MATRIX_PACKED_MATRIX_H_
#define SLAB_MATRIX_PACKED_MATRIX_H_

#include "slab/matrix/error.h"

// Triangular matrix type
struct lower_tag {};
struct upper_tag {};
struct unit_lower_tag : public lower_tag {};
struct unit_upper_tag : public upper_tag {};

struct upper {
  using triangular_type = upper_tag;

  upper();
  upper(std::size_t n) {
    extents[0] = n;
    extents[1] = n;
    size = std::size_t((1 + n) * n / 2);
  }

  inline bool other_half(std::size_t i, std::size_t j) const {
    return i > j;
  }

  inline std::size_t operator()(std::size_t i, std::size_t j) const {
    return (2 * extents[0] - i + 1) * i / 2 + (j - i);
  }

  std::size_t size;
  std::array<std::size_t, 2> extents;
};

struct unit_upper : public upper {
  using triangular_type = unit_upper_tag;
  unit_upper(std::size_t n) : upper(n) { size -= n; }
};

struct lower {
  using triangular_type = lower_tag;

  lower();
  lower(std::size_t n) {
    extents[0] = n;
    extents[1] = n;
    size = std::size_t((1 + n) * n / 2);
  }

  inline bool other_half(std::size_t i, std::size_t j) const {
    return j > i;
  }

  inline std::size_t operator()(std::size_t i, std::size_t j) const {
    return (1 + i) * i / 2 + j;
  }

  std::size_t size;
  std::array<std::size_t, 2> extents;
};

struct unit_lower : public lower {
  using triangular_type = unit_lower_tag;
  unit_lower(std::size_t n) : lower(n) { size -= n; }
};

template<typename T>
struct is_upper : public std::false_type {};

template<>
struct is_upper<upper> : public std::true_type {};

template<typename T>
struct is_lower : public std::false_type {};

template<>
struct is_lower<lower> : public std::true_type {};

template<typename T, typename TRI>
class PackedMatrix {
 public:
  using value_type = T;
  using iterator = typename std::vector<T>::iterator;
  using const_iterator  = typename std::vector<T>::const_iterator;
  using triangular_type = typename TRI::triangular_type;

  PackedMatrix() = default;
  PackedMatrix(PackedMatrix &&) = default;
  PackedMatrix &operator=(PackedMatrix &&) = default;
  PackedMatrix(PackedMatrix const &) = default;
  PackedMatrix &operator=(PackedMatrix const &) = default;

  PackedMatrix(std::size_t n) : elems_(n * n), desc_(n) {}
  PackedMatrix(std::size_t n, const Matrix<T, 1> &v) : desc_(n) {
    if (v.size() != desc_.size)
      err_quit("Fail to construct a packed matrix, the size of vector provided is incorrect");

    init(v, triangular_type());
  }

  const TRI &descriptor() const { return desc_; }

  //! "flat" element access
  ///@{
  T *data() { return elems_.data(); }
  const T *data() const { return elems_.data(); }
  ///@}

  std::size_t n_rows() const { return desc_.extents[0]; }
  std::size_t n_cols() const { return desc_.extents[1]; }

 protected:
  std::vector<T> elems_;
  TRI desc_;

 private:
  void init(const Matrix<T, 1> &v, upper_tag) { elems_.assign(v.begin(), v.end()) ; }
  void init(const Matrix<T, 1> &v, lower_tag) { elems_.assign(v.begin(), v.end()) ; }
  void init(const Matrix<T, 1> &v, unit_upper_tag) {
    for (std::size_t j = 0, index = 0; j != desc_.extents[1]; ++j) {
      for (std::size_t i = j; i != desc_.extents[0]; ++i) {
        if (i == j) elems_.push_back(T{1});
        else elems_.push_back(v(index++));
      }
    }
  }
  void init(const Matrix<T, 1> &v, unit_lower_tag) {
    for (std::size_t i = 0, index = 0; i != desc_.extents[0]; ++i) {
      for (std::size_t j = 0; j <= i; ++j) {
        if (i == j) elems_.push_back(T{1});
        else elems_.push_back(v(index++));
      }
    }
  }
};

#endif // SLAB_MATRIX_PACKED_MATRIX_H
