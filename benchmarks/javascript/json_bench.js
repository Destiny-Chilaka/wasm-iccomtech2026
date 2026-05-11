const RUNS = 100;
const NUM_RECORDS = 5000;

// Build synthetic JSON records
const records = [];
for (let i = 0; i < NUM_RECORDS; i++) {
    records.push(JSON.stringify({
        id: i,
        value: parseFloat((i * 0.1234).toFixed(4)),
        label: `record_${i}_label_data_padding_text`
    }));
}
const jsonData = records.join('\n');

// warmup
let warmup = 0;
for (let i = 0; i < NUM_RECORDS; i++) warmup += JSON.parse(records[i]).id;

let sink = 0;
const start = performance.now();
for (let r = 0; r < RUNS; r++) {
    for (let i = 0; i < NUM_RECORDS; i++) {
        const obj = JSON.parse(records[i]);
        sink += obj.id;
    }
}
const end = performance.now();

console.log(`JSON Parse JS mean: ${((end - start) / RUNS).toFixed(2)} ms (sink: ${sink})`);
