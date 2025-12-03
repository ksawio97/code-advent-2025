#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef enum {L = -1, R = 1} direction;

typedef struct {
    direction dir;
    int val;
} Rotation;

// returns list count
int read_rotations(FILE* fh, Rotation** rotations_start) {
    int rotation_count = 0, max_size = 1;
    Rotation* rotations = malloc(max_size * sizeof(Rotation));
    
    size_t buff_size = 32;
    char* line = malloc(buff_size);
    size_t chars_count;
    while (fgets(line, buff_size, fh)) {
        // allocate more memory for list
        if (rotation_count >= max_size) {
            max_size *= 2;

            rotations = realloc(rotations, sizeof(Rotation[max_size]));
        }

        rotations[rotation_count].dir = line[0] == 'L' ? -1 : 1;
        rotations[rotation_count].val = atoi(line + 1);

        rotation_count++;
    }
    // set min size for rotations
    rotations = realloc(rotations, sizeof(Rotation[rotation_count]));
    *rotations_start = rotations;
    
    free(line);

    return rotation_count;
}

int p1(const int rotations_count, Rotation rotations[rotations_count]) {
    int zero_result_count = 0;
    int curr_val = 50;
    for (int i = 0; i < rotations_count; i++) {
        curr_val += rotations[i].dir * rotations[i].val;
        
        curr_val = ((curr_val % 100 + 100) % 100);
        // might change to 0 after altercation
        if (curr_val == 0) {
            zero_result_count++;
        }
    }

    return zero_result_count;
}

int p2(const int rotations_count, Rotation rotations[rotations_count]) {
    int zero_result_count = 0;
    int curr_val = 50;
    for (int i = 0; i < rotations_count; i++) {
        curr_val += rotations[i].dir * rotations[i].val;
        // TODO fix counting logic:

        // count while rotating 
        if (curr_val < 0 && (curr_val % 100) != 0 && abs(curr_val) != rotations[i].val) {
            zero_result_count++;
        }
        zero_result_count += abs(curr_val) / 100;

        curr_val = ((curr_val % 100 + 100) % 100);
        if (curr_val == 0) {
            zero_result_count++;
        }
    }

    return zero_result_count;
}
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "You need to provide input file path\n");
        return 0;
    }
    char* in_data = argv[1];
    FILE* fh = fopen(in_data, "r+");
    
    Rotation* rotations;
    int rotations_count = read_rotations(fh, &rotations);
    printf("P1: %d\n", p1(rotations_count, rotations));
    printf("P2: %d\n", p2(rotations_count, rotations));

    return 1;
}
