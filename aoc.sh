#!/bin/bash

set -e

function usage() {
  echo "Usage:"
  echo "  ./aoc.sh build <year> <day> <part> [input|test] [run]"
  echo "  ./aoc.sh benchmark <year> <day> <part> [input|test] [runs]"
  echo "  ./aoc.sh aoc <input|prompt|submit> [<year> <day> <part> [input|test] [run]]"
  echo ""
  echo "Examples:"
  echo "  ./aoc.sh build 2024 01 1 input run"
  echo "  ./aoc.sh benchmark 2024 01 2 test 500"
  echo "  ./aoc.sh aoc submit 2024 01 2 input run"
  exit 1
}

# Build tool
function build_tool() {
  g++ -std=c++11 -o build/build build/build.cpp
}

# Benchmark tool
function build_benchmark() {
  g++ -std=c++11 -o benchmark/benchmark benchmark/benchmark.cpp
}

# AoC tool
function build_aoc() {
  g++ -std=c++11 -lcurl -o aoc/aoc aoc/aoc.cpp
}

# Run build and optionally run the solution
function run_build() {
  build_tool
  ./build/build "$@"
}

# Run benchmark
function run_benchmark() {
  build_benchmark
  ./benchmark/benchmark "$@"
}

# Run AoC tool after optionally running build
function run_aoc() {
  build_tool
  build_aoc

  action="$1"
  shift

  # If the user provided year/day/part (and optionally input/test/run)
  if [[ $# -ge 3 ]]; then
    ./build/build "$@" > /dev/null
  fi

  ./aoc/aoc "$action"
}

# Main dispatch
case "$1" in
  build)
    shift
    run_build "$@"
    ;;
  benchmark)
    shift
    run_benchmark "$@"
    ;;
  aoc)
    shift
    run_aoc "$@"
    ;;
  *)
    usage
    ;;
esac
