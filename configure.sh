#! /bin/sh
. /opt/intel/oneapi/setvars.sh --include-intel-llvm; 
cmake -S . -B build -DENABLE_SYCL=ON;
