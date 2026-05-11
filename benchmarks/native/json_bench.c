#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_RECORDS 5000

// Simulates parsing JSON-like key:value records
typedef struct {
    int id;
    float value;
    char label[32];
} Record;

Record records[NUM_RECORDS];

void parse_json(const char *data, size_t len) {
    int id = 0;
    float val = 0;
    char label[32];
    size_t i = 0;
    int rec = 0;

    while (i < len && rec < NUM_RECORDS) {
        // scan for id
        while (i < len && (data[i] < '0' || data[i] > '9')) i++;
        id = 0;
        while (i < len && data[i] >= '0' && data[i] <= '9')
            id = id * 10 + (data[i++] - '0');

        // scan for value
        while (i < len && data[i] != ':') i++;
        i++;
        val = 0; float dec = 0.1f; int inDec = 0;
        while (i < len && (data[i]=='.' || (data[i]>='0'&&data[i]<='9'))) {
            if (data[i]=='.') { inDec=1; i++; continue; }
            if (!inDec) val = val*10+(data[i]-'0');
            else { val += (data[i]-'0')*dec; dec*=0.1f; }
            i++;
        }

        // scan for label
        while (i < len && data[i] != '"') i++;
        i++;
        int li = 0;
        while (i < len && data[i] != '"' && li < 31)
            label[li++] = data[i++];
        label[li] = '\0';

        records[rec].id = id;
        records[rec].value = val;
        strncpy(records[rec].label, label, 31);
        rec++;
    }
}

int main() {
    // Build a synthetic JSON-like string (~10MB)
    size_t chunk = 2000;
    size_t total = chunk * NUM_RECORDS;
    char *data = (char*)malloc(total);
    size_t pos = 0;
    for (int i = 0; i < NUM_RECORDS; i++) {
        int written = snprintf(data + pos, total - pos,
            "{\"id\":%d,\"value\":%.4f,\"label\":\"record_%d_label_data_padding_text\"}\n",
            i, (float)i * 0.1234f, i);
        pos += written;
    }

    int RUNS = 100;
    struct timespec start, end;
    volatile int sink = 0;

    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int r = 0; r < RUNS; r++) {
        parse_json(data, pos);
        sink += records[0].id;
    }
    clock_gettime(CLOCK_MONOTONIC, &end);

    double ms = ((end.tv_sec - start.tv_sec)*1e9 + (end.tv_nsec - start.tv_nsec)) / 1e6 / RUNS;
    printf("JSON Parse mean: %.2f ms (sink: %d)\n", ms, sink);
    free(data);
    return 0;
}
