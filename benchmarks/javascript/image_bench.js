const RUNS = 20;
const WIDTH = 1024;
const HEIGHT = 1024;

const input = new Uint8Array(HEIGHT * WIDTH * 3);
const output = new Uint8Array(HEIGHT * WIDTH * 3);

for (let y = 0; y < HEIGHT; y++)
    for (let x = 0; x < WIDTH; x++) {
        const i = (y * WIDTH + x) * 3;
        input[i]   = (x + y) % 256;
        input[i+1] = (x * 2) % 256;
        input[i+2] = (y * 3) % 256;
    }

const kernel = [[1,2,1],[2,4,2],[1,2,1]];

function gaussianBlur() {
    for (let y = 1; y < HEIGHT-1; y++) {
        for (let x = 1; x < WIDTH-1; x++) {
            for (let c = 0; c < 3; c++) {
                let sum = 0;
                for (let ky = -1; ky <= 1; ky++)
                    for (let kx = -1; kx <= 1; kx++)
                        sum += input[((y+ky)*WIDTH+(x+kx))*3+c] * kernel[ky+1][kx+1];
                output[(y*WIDTH+x)*3+c] = sum >> 4;
            }
        }
    }
}

// warmup
gaussianBlur();

let sink = 0;
const start = performance.now();
for (let r = 0; r < RUNS; r++) {
    gaussianBlur();
    sink += output[(512*WIDTH+512)*3];
}
const end = performance.now();

console.log(`Image Filter JS mean: ${((end - start) / RUNS).toFixed(2)} ms (sink: ${sink})`);
