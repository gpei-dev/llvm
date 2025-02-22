//==------------- View.h - Non-owning view to contiguous memory ------------==//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef SYCL_FUSION_COMMON_VIEW_H
#define SYCL_FUSION_COMMON_VIEW_H

#include <cstddef>
#include <type_traits>

namespace jit_compiler {

/// Read-only, non-owning view of a linear sequence of \p T.
template <typename T> class View {
public:
  constexpr View(const T *Ptr, size_t Size) : Ptr(Ptr), Size(Size) {}

  template <template <typename...> typename C>
  explicit constexpr View(const C<T> &Cont)
      : Ptr(Cont.data()), Size(Cont.size()) {}

  constexpr const T *begin() const { return Ptr; }
  constexpr const T *end() const { return Ptr + Size; }
  constexpr size_t size() const { return Size; }

  template <template <typename...> typename C> auto to() const {
    return C<T>{begin(), end()};
  }

private:
  const T *const Ptr;
  const size_t Size;
};

// Deduction guide
template <template <typename...> typename C, typename T>
View(const C<T> &) -> View<T>;

} // namespace jit_compiler

#endif // SYCL_FUSION_COMMON_VIEW_H
