#!/bin/sh
find src/ -iname "*.cpp" -exec clang-format -i {} \;
find src/ -iname "*.h" -exec clang-format -i {} \;
