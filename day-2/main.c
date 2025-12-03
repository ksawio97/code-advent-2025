#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    long long start;
    long long end;
} Range;

void get_val(FILE* fh, long long* val) {
    *val = 0;

    char c;
    // collect range start
    while ((c = fgetc(fh)) >= 48 && c <= 57) {
        *val = *val * 10 + (c - 48/* From ascii to dec */);
    }
}

int peek_char(FILE* fh) {
    int c = fgetc(fh); 

    if (c != EOF) {
        // If successful, push it back onto the stream
        ungetc(c, fh);
    }
    
    return c;
}

int read_ranges(FILE* fh, Range** ranges_start) {
    int max_size = 1, size = 0;
    Range* ranges = malloc(sizeof(Range) * max_size);
    
    char c;   
    while (peek_char(fh) != EOF) {
        if (size == max_size) {
            max_size *= 2;
            ranges = realloc(ranges, sizeof(Range) * max_size);
        }
        // get start
        get_val(fh, &(ranges[size].start));
        // get end 
        get_val(fh, &(ranges[size].end));
        
        size++;
    }
    // move to our list 
    *ranges_start = realloc(ranges, sizeof(Range) * max_size); 

    return size;
}

int get_digits_count(long long val) {
    int i = 0;
    while (val > 0) {
        val /= 10;
        i++;
    }

    return i;
}

bool is_num_duplicate(long long num, const int digits_count) {

    const int digits_half = digits_count / 2;
    for (long long i = 10; i < pow(10,digits_half + 1); i *= 10) {
        long long j = i * pow(10, digits_half);
        // gets digit i and j and checks if they are different
        if (num % i / (i / 10) != num % j / (j / 10)) {
            return false;
        }
    }

    return true;
}

long long p1(const int ranges_count, Range ranges[ranges_count]) {
    long long sum = 0L;
    
    for (int i = 0; i < ranges_count; i++) {
        for (long long num = ranges[i].start; num < ranges[i].end + 1; num++) {
            // odd nums can't be duplicates
            int digits_count = get_digits_count(num);
            if (digits_count % 2 == 1) {
                continue;
            }

            if (is_num_duplicate(num, digits_count)) {
                sum += num;
            }
        }
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

    Range* ranges;
    const int ranges_count = read_ranges(fh, &ranges);
    
    printf("Part 1: %lld\n", p1(ranges_count, ranges));

    return 1;
}
