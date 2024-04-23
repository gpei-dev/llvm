// REQUIRES: linux, cpu
// RUN: %{build} %device_asan_flags -g -O0 -o %t.out
// RUN: env SYCL_PREFER_UR=1 %{run} not %t.out 2>&1 | FileCheck %s
// RUN: %{build} %device_asan_flags -g -O1 -o %t.out
// RUN: env SYCL_PREFER_UR=1 %{run} not %t.out 2>&1 | FileCheck %s
// RUN: %{build} %device_asan_flags -g -O2 -o %t.out
// RUN: env SYCL_PREFER_UR=1 %{run} not %t.out 2>&1 | FileCheck %s
#include <sycl/sycl.hpp>
#include <vector>

constexpr std::size_t N = 8;
constexpr std::size_t group_size = 4;

__attribute__((noinline)) void foo(int *data1, sycl::local_accessor<int> acc1,
                                   sycl::local_accessor<int> acc2,
                                   sycl::local_accessor<int> acc3,
                                   sycl::accessor<int> buf1,
                                   sycl::nd_item<1> item) {
  data1[item.get_global_id()] =
      acc1[item.get_local_id()] + acc2[item.get_local_id()] +
      acc3[item.get_local_id() + 1] + buf1[item.get_global_id()];
}
// CHECK: ERROR: DeviceSanitizer: out-of-bounds-access on Local Memory
// CHECK: READ of size 4 at kernel {{<.*MyKernel>}} LID(3, 0, 0) GID({{.*}}, 0, 0)
// CHECK:   #0 {{foo.*}} {{.*local_accessor_function.cpp}}:[[@LINE-4]]

int main() {
  sycl::queue Q;
  auto data1 = sycl::malloc_device<int>(N, Q);
  auto data2 = sycl::malloc_device<int>(N, Q);
  auto vec = std::vector<int>(N);
  auto buf = sycl::buffer<int>(vec.data(), N);

  Q.submit([&](sycl::handler &cgh) {
    auto acc1 = sycl::local_accessor<int>(group_size, cgh);
    auto buf1 = buf.get_access<sycl::access::mode::read_write>(cgh);
    auto acc2 = sycl::local_accessor<int>(group_size, cgh);
    auto buf2 = buf.get_access<sycl::access::mode::read_write>(cgh);
    auto acc3 = sycl::local_accessor<int>(group_size, cgh);
    auto buf3 = buf.get_access<sycl::access::mode::read_write>(cgh);
    cgh.parallel_for<class MyKernel>(
        sycl::nd_range<1>(N, group_size), [=](sycl::nd_item<1> item) {
          data2[item.get_global_id()] =
              acc1[item.get_local_id()] + acc2[item.get_local_id()] +
              acc3[item.get_local_id()] + buf1[item.get_local_id()] +
              buf3[item.get_local_id()];
          foo(data1, acc1, acc2, acc3, buf1, item);
        });
  });

  Q.wait();
  return 0;
}
