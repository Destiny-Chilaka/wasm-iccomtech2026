#!/bin/bash

RUNS=20
NATIVE_TOTAL=0
WASM_TOTAL=0
NODE_TOTAL=0

# Minimal C program — just starts and exits
cat > coldstart.c << 'CSRC'
#include <stdio.h>
int main() { printf("ok\n"); return 0; }
CSRC

gcc -O3 -o coldstart_native coldstart.c
emcc -O3 -o coldstart_wasm.js coldstart.c -s SINGLE_FILE=1

echo "Running $RUNS cold start measurements..."

for i in $(seq 1 $RUNS); do
    # Native
    START=$(date +%s%N)
    ./coldstart_native > /dev/null
    END=$(date +%s%N)
    NATIVE_TOTAL=$((NATIVE_TOTAL + (END - START)))

    # Wasm via Node
    START=$(date +%s%N)
    node coldstart_wasm.js > /dev/null
    END=$(date +%s%N)
    WASM_TOTAL=$((WASM_TOTAL + (END - START)))

    # Pure Node.js
    START=$(date +%s%N)
    node -e "console.log('ok')" > /dev/null
    END=$(date +%s%N)
    NODE_TOTAL=$((NODE_TOTAL + (END - START)))
done

echo "Cold Start Native mean:  $(echo "scale=2; $NATIVE_TOTAL / $RUNS / 1000000" | bc) ms"
echo "Cold Start Wasm mean:    $(echo "scale=2; $WASM_TOTAL / $RUNS / 1000000" | bc) ms"
echo "Cold Start Node.js mean: $(echo "scale=2; $NODE_TOTAL / $RUNS / 1000000" | bc) ms"
