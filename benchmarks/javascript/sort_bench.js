const RUNS = 100;
const SIZE = 100000;

// Seeded random for reproducibility
function seededRand(seed) {
    const arr = new Int32Array(SIZE);
    let s = seed;
    for (let i = 0; i < SIZE; i++) {
        s = (s * 1664525 + 1013904223) & 0x7fffffff;
        arr[i] = s % 1000000;
    }
    return arr;
}

const original = seededRand(42);
const arr = new Int32Array(SIZE);

// warmup
arr.set(original);
arr.sort((a, b) => a - b);

let sink = 0;
const start = performance.now();
for (let r = 0; r < RUNS; r++) {
    arr.set(original);
    arr.sort((a, b) => a - b);
    sink += arr[0];
}
const end = performance.now();

console.log(`Integer Sort JS mean: ${((end - start) / RUNS).toFixed(2)} ms (sink: ${sink})`);
