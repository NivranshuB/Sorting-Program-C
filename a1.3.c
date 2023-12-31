/*
    The Hybrid Merge Sort to use for Operating Systems Assignment 1 2021
    written by Robert Sheehan

    Modified by: Nivranshu Bose
    UPI: nbos433

    By submitting a program you are claiming that you and only you have made
    adjustments and additions to this code.
 */

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h>
#include <sys/resource.h>
#include <stdbool.h>
#include <sys/time.h>
#include <sys/times.h>
#include <math.h>
#include <pthread.h>

#define SIZE    4
#define MAX     1000
#define SPLIT   16

int thread_number = 1;

struct block {
    int size;
    int *data;
};

void print_data(struct block *block) {
    for (int i = 0; i < block->size; ++i)
        printf("%d ", block->data[i]);
    printf("\n");
}

/* The insertion sort for smaller halves. */
void insertion_sort(struct block *block) {
    for (int i = 1; i < block->size; ++i) {
        for (int j = i; j > 0; --j) {
            if (block->data[j-1] > block->data[j]) {
                int temp;
                temp = block->data[j-1];
                block->data[j-1] = block->data[j];
                block->data[j] = temp;
            }
        }
    }
}

/* Combine the two halves back together. */
void merge(struct block *left, struct block *right) {
    int *combined = calloc(left->size + right->size, sizeof(int));
    if (combined == NULL) {
        perror("Allocating space for merge.\n");
        exit(EXIT_FAILURE);
    }
        int dest = 0, l = 0, r = 0;
        while (l < left->size && r < right->size) {
                if (left->data[l] < right->data[r])
                        combined[dest++] = left->data[l++];
                else
                        combined[dest++] = right->data[r++];
        }
        while (l < left->size)
                combined[dest++] = left->data[l++];
        while (r < right->size)
                combined[dest++] = right->data[r++];
    memmove(left->data, combined, (left->size + right->size) * sizeof(int));
    free(combined);
}

/* Merge sort the data. */
void *merge_sort(void *addr) {
    struct block *block = (struct block*)addr;
    if (block->size > SPLIT) {
        struct block left_block;
        struct block right_block;
        left_block.size = block->size / 2;
        left_block.data = block->data;
        right_block.size = block->size - left_block.size; // left_block.size + (block->size % 2);
        right_block.data = block->data + left_block.size;

        merge_sort(&left_block);
        merge_sort(&right_block);
        merge(&left_block, &right_block);
    } else {
        insertion_sort(block);
    }

    return NULL;
}

/* Check to see if the data is sorted. */
bool is_sorted(struct block *block) {
    bool sorted = true;
    for (int i = 0; i < block->size - 1; i++) {
        if (block->data[i] > block->data[i + 1])
            sorted = false;
    }
    return sorted;
}

/* Fill the array with random data. */
void produce_random_data(struct block *block) {
    srand(1); // the same random data seed every time
    for (int i = 0; i < block->size; i++) {
        block->data[i] = rand() % MAX;
    }
}

int main(int argc, char *argv[]) {
        long size;

        if (argc < 2) {
                size = SIZE;
        } else {
                size = atol(argv[1]);
        }
    struct block block;
    block.size = (int)pow(2, size);
    block.data = (int *)calloc(block.size, sizeof(int));
    if (block.data == NULL) {
        perror("Unable to allocate space for data.\n");
        exit(EXIT_FAILURE);
    }

    produce_random_data(&block);

    struct timeval start_wall_time, finish_wall_time, wall_time;
    struct tms start_times, finish_times;
    gettimeofday(&start_wall_time, NULL);
    times(&start_times);

    struct block block_one;
    struct block block_two;
    struct block block_three;
    struct block block_four;
    struct block block_five;
    struct block block_six;
    struct block block_seven;
    struct block block_eight;

    block_one.size = block.size / 8;
    block_one.data = block.data;

    block_two.size = block.size / 8;
    block_two.data = block.data + block_one.size;

    block_three.size = block.size / 8;
    block_three.data = block.data + (block_one.size * 2);

    block_four.size = block.size / 8;
    block_four.data = block.data + (block_one.size * 3);

    block_five.size = block.size / 8;
    block_five.data = block.data + (block_one.size * 4);

    block_six.size = block.size / 8;
    block_six.data = block.data + (block_one.size * 5);

    block_seven.size = block.size / 8;
    block_seven.data = block.data + (block_one.size * 6);
        
    block_eight.size = block.size - (block_one.size * 7);
    block_eight.data = block.data + (block_one.size * 7);

    int const num_threads = 7;
    pthread_t thread_refs[num_threads];

    pthread_create(&thread_refs[0], NULL, merge_sort, &block_two);
    pthread_create(&thread_refs[1], NULL, merge_sort, &block_three);
    pthread_create(&thread_refs[2], NULL, merge_sort, &block_four);
    pthread_create(&thread_refs[3], NULL, merge_sort, &block_five);
    pthread_create(&thread_refs[4], NULL, merge_sort, &block_six);
    pthread_create(&thread_refs[5], NULL, merge_sort, &block_seven);
    pthread_create(&thread_refs[6], NULL, merge_sort, &block_eight);

    merge_sort(&block_one);

    pthread_join(thread_refs[0], NULL);  
    pthread_join(thread_refs[1], NULL);
    pthread_join(thread_refs[2], NULL);
    pthread_join(thread_refs[3], NULL);
    pthread_join(thread_refs[4], NULL);
    pthread_join(thread_refs[5], NULL);
    pthread_join(thread_refs[6], NULL);

    merge(&block_one, &block_two);
    merge(&block_three, &block_four);
    merge(&block_five, &block_six);
    merge(&block_seven, &block_eight);

    block_one.size = block.size / 4;
    block_one.data = block.data;

    block_two.size = block.size / 4;
    block_two.data = block.data + block_one.size;

    block_three.size = block.size / 4;
    block_three.data = block.data + (block_one.size * 2);

    block_four.size = block.size - (block_one.size * 3);
    block_four.data = block.data + (block_one.size * 3);

    pthread_create(&thread_refs[0], NULL, merge_sort, &block_two);
    pthread_create(&thread_refs[1], NULL, merge_sort, &block_three);
    pthread_create(&thread_refs[2], NULL, merge_sort, &block_four);

    merge_sort(&block_one);

    pthread_join(thread_refs[0], NULL);  
    pthread_join(thread_refs[1], NULL);
    pthread_join(thread_refs[2], NULL);

    merge(&block_one, &block_two);
    merge(&block_three, &block_four);

    block_one.size = block.size / 2;
    block_one.data = block.data;

    block_two.size = block.size - block_one.size;
    block_two.data = block.data + block_one.size;

    pthread_create(&thread_refs[0], NULL, merge_sort, &block_two);

    merge_sort(&block_one);

    pthread_join(thread_refs[0], NULL); 

    merge(&block_one, &block_two);

    gettimeofday(&finish_wall_time, NULL);
    times(&finish_times);
    timersub(&finish_wall_time, &start_wall_time, &wall_time);
    printf("start time in clock ticks: %ld\n", start_times.tms_utime);
    printf("finish time in clock ticks: %ld\n", finish_times.tms_utime);
    printf("wall time %ld secs and %ld microseconds\n", wall_time.tv_sec, wall_time.tv_usec);

    if (block.size < 1025)
        print_data(&block);

    printf(is_sorted(&block) ? "sorted\n" : "not sorted\n");
    free(block.data);
    exit(EXIT_SUCCESS);
}