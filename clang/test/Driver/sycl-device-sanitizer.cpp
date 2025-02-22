// RUN: %clangxx -fsycl -fsanitize=address -c %s -### 2>&1 \
// RUN:   | FileCheck --check-prefix=SYCL-ASAN %s
// SYCL-ASAN: clang{{.*}} "-fsycl-is-device"
// SYCL-ASAN-SAME: -fsanitize=address
// SYCL-ASAN-SAME: -fsanitize-address-use-after-return=never
// SYCL-ASAN-SAME: -fno-sanitize-address-use-after-scope
// SYCL-ASAN-SAME: "-mllvm" "-asan-instrumentation-with-call-threshold=0"
// SYCL-ASAN-SAME: "-mllvm" "-asan-constructor-kind=none"
// SYCL-ASAN-SAME: "-mllvm" "-asan-stack=0"
// SYCL-ASAN-SAME: "-mllvm" "-asan-globals=0"

// RUN: %clangxx -fsycl -fsanitize=address -mllvm -asan-stack=1 -c %s -### 2>&1 \
// RUN:   | FileCheck --check-prefix=SYCL-ASAN-FILTER %s
// SYCL-ASAN-FILTER: clang{{.*}} "-fsycl-is-device"
// SYCL-ASAN-FILTER-SAME: -fsanitize=address
// SYCL-ASAN-FILTER-SAME: "-mllvm" "-asan-stack=0"
