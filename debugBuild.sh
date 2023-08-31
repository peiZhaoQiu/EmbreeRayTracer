#! /bin/sh


cmake -S . -B debugBuild -DENABLE_DEBUG=ON;
make -C debugBuild