const N = 256;
const RUNS = 100;

const A = Array.from({length: N}, (_, i) => Float32Array.from({length: N}, (_, j) => (i+j)/N));
const B = Array.from({length: N}, (_, i) => Float32Array.from({length: N}, (_, j) => (i*j)/N));
const C = Array.from({length: N}, () => new Float32Array(N));

function matMul() {
    for (let i = 0; i < N; i++)
        for (let k = 0; k < N; k++)
            for (let j = 0; j < N; j++)
                C[i][j] += A[i][k] * B[k][j];
}

// warmup
matMul();

let sink = 0;
const start = performance.now();
for (let r = 0; r < RUNS; r++) {
    matMul();
    sink += C[0][0];
}
const end = performance.now();

console.log(`Matrix JS mean: ${((end-start)/RUNS).toFixed(2)} ms (sink: ${sink.toFixed(4)})`);
