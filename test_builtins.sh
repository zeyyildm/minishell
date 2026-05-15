#!/bin/bash

echo "=== TEST 1: echo ==="
echo "Test: echo hello"
echo hello

echo -e "\nTest: echo -n hello"
echo -n hello
echo ""

echo -e "\nTest: echo hello world"
echo hello world

echo -e "\n\n=== TEST 2: pwd ==="
pwd

echo -e "\n\n=== TEST 3: export/env ==="
echo "Test: export TEST_VAR=hello"
export TEST_VAR=hello
echo "Test: env | grep TEST_VAR"
env | grep TEST_VAR

echo -e "\n\n=== TEST 4: cd ==="
echo "Current dir: $(pwd)"
cd /tmp
echo "After cd /tmp: $(pwd)"
cd -
echo "After cd -: $(pwd)"

echo -e "\n\n=== TEST 5: unset ==="
echo "Before unset TEST_VAR: $TEST_VAR"
unset TEST_VAR
echo "After unset TEST_VAR: $TEST_VAR (should be empty)"

echo -e "\n\n=== TEST 6: exit ==="
echo "exit tests (exit 0 exits shell, testing syntax only)"
