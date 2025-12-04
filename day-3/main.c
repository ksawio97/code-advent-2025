#include <stdio.h>
#include <stdlib.h>

int read_data(FILE* fh, int** nums, int* row_size) {
    int max_size = 1, size = 0;
    *nums = malloc(max_size * sizeof(int));
    char c;
    // read 1th line
    while ((c = fgetc(fh)) != '\n') {
        if (max_size == size) {
            max_size *= 2;
            *nums = realloc(*nums, max_size * sizeof(int));
        }

        (*nums)[size] = c - 48;
    size++;
    }
    *row_size = size;
    while ((c = fgetc(fh)) != -1) {
        if (c == '\n') {
            continue;
        }

        if (max_size == size) {
            max_size *= 2;
            *nums = realloc(*nums, max_size * sizeof(int));
        }

        (*nums)[size] = c - 48;
        size++;
    }
    nums = realloc(*nums, size * sizeof(int));

    return size;
}

int find_biggest_index(const int n, int nums[n]) {
    if (n == 0) {
        return -1;
    }
    int biggest_index = 0;
    for (int i = 1; i < n; i++) {
        if (nums[i] > nums[biggest_index]) {
            biggest_index = i;
        }
    }

    return biggest_index;
}

int p1(const int size, const int row_size, int nums[size]) {
    int sum = 0;

    for (int r = 0; r < size; r += row_size) {
        int first_index = find_biggest_index(row_size, nums + r);
        // last digit is biggest, can't choose it bcs 2 digit is always bigger 
        if (first_index == row_size - 1) {
            first_index = find_biggest_index(row_size - 1, nums + r);
        }

        int second_index = find_biggest_index(row_size - first_index - 1, nums + r + first_index + 1);

        sum += nums[r + first_index] * 10 + nums[r + first_index + second_index + 1];
    }
    
    return sum;
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "You need to provide input file path\n");
        return 0;
    }
    char* in_data = argv[1];
    FILE* fh = fopen(in_data, "r+");

    if (fh == NULL) {
        fprintf(stderr, "File read error\n");
        return 0;
    }
    int row_size;
    int* nums;
    int nums_count = read_data(fh, &nums, &row_size); 

    printf("Part 1: %d\n", p1(nums_count, row_size, nums));
    // printf("Part 2: %lld\n", p2(ranges_count, ranges));
    return 1;
}
