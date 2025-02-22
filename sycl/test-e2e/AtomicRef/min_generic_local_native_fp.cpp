// RUN: %{build} -o %t.out
// RUN: %{run} %t.out

#define SYCL_USE_NATIVE_FP_ATOMICS
#define FP_TESTS_ONLY
#define TEST_GENERIC_IN_LOCAL 1

#include "min.h"

int main() { min_test_all<access::address_space::generic_space>(); }
