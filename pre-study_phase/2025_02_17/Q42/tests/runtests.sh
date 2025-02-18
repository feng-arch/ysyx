#!/bin/sh
SCRIPT_DIR=$(cd "$(dirname "$0")" && pwd)
cd "$SCRIPT_DIR"
valgrind ./stack_tests.out
valgrind ./queue_tests.out
