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

// [from, to] range is inclusive
long long cut_num(long long num, int from, int to, const int digits_count) {
    int omit_digits = digits_count - to - 1;
    int cut_size = to - from + 1;

    return (long long)(num / pow(10, omit_digits)) % (long long)pow(10, cut_size);
}

bool is_made_by_sequence(long long num, const int digits_count) {
    for (int seq_size = digits_count / 2; seq_size > 0; seq_size--) {
        // can't be made by seq of this size
        if (digits_count % seq_size != 0) {
            continue;
        }
        
        bool found_unique = false;
        for (int start = 0; start < digits_count - seq_size; start += seq_size) {
            const long long num1 = cut_num(num, start, start + seq_size - 1, digits_count), num2 = cut_num(num, start + seq_size, start + seq_size * 2 - 1, digits_count);
            if (num1 != num2) {
                found_unique = true;
                break;
            }    
        }

        if (!found_unique) {
            return true;
        } 
    }

    return false;
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

long long p2(const int ranges_count, Range ranges[ranges_count]) {
    long long sum = 0L;


    for (int i = 0; i < ranges_count; i++) {
        for (long long num = ranges[i].start; num < ranges[i].end + 1; num++) {
            int digits_count = get_digits_count(num);

            if (is_made_by_sequence(num, digits_count)) {
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
    printf("Part 2: %lld\n", p2(ranges_count, ranges));

    return 1;
}
