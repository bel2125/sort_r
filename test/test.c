#include "../src/sort_r.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void *g_userarg;


static uint64_t get_timetamp_ns(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * (uint64_t)1000000000 + (uint64_t)ts.tv_nsec;
}


static int qcomparefunc(const void *p1, const void *p2)
{
    int *i1 = (int*)p1;
    int *i2 = (int*)p2;
    if (*i1 < *i2) return -1;
    if (*i1 > *i2) return +1;
    return 0;
}


static int compfunc(const void *p1, const void *p2, void *userarg)
{
    assert(g_userarg == userarg);
    return qcomparefunc(p1, p2);
}


static int compfunc_timing(const void *p1, const void *p2, void *userarg)
{
    (void)userarg;
    int *i1 = (int*)p1;
    int *i2 = (int*)p2;
    if (*i1 < *i2) return -1;
    if (*i1 > *i2) return +1;
    return 0;
}


static void randomtest(int n)
{
    g_userarg = (char*)0 + n;
    int *data_sort_r = (int*)calloc(n, sizeof(int));
    int *data_qsort = (int*)calloc(n, sizeof(int));
    assert(data_sort_r != NULL);
    assert(data_qsort != NULL);

    for (int i=0; i<n; i++) {
        data_qsort[i] = data_sort_r[i] = random();
    }

    printf("Sorting %i elements: ", n);
    uint64_t ts, te;
    unsigned long dtq, dts;
    
    ts = get_timetamp_ns();
    qsort(data_qsort, n, sizeof(int), qcomparefunc);
    te = get_timetamp_ns();
    dtq = (unsigned long)(te - ts);
    printf("qsort: %lu (%.2f), ", dtq, (double)dtq/(double)n);
    
    ts = get_timetamp_ns();
    sort_r(data_sort_r, n, sizeof(int), compfunc_timing, g_userarg);
    te = get_timetamp_ns();
    dts = (unsigned long)(te - ts);
    printf("sort_r: %lu (%.2f), ", dts, (double)dts/(double)n);

    double rela = (double)dts / (double)(dts + dtq);
    if (rela > 0.6) {printf("qsort faster, ");}
    else if (rela < 0.4) {printf("sort_r faster, ");}
    else {printf("same speed, ");}

    for (int i=0; i<n; i++) {
        assert(data_qsort[i] == data_sort_r[i]);
    }
    printf("compare: OK\n");

    free(data_qsort);
    free(data_sort_r);
}


static void sequencetest(int n)
{
    g_userarg = (char*)0 + n;
    int *data_sorted = (int*)calloc(n, sizeof(int));
    int *data_sort_r = (int*)calloc(n, sizeof(int));
    assert(data_sorted != NULL);
    assert(data_sort_r != NULL);

    for (int i=0; i<n; i++) {
        data_sort_r[i] = data_sorted[i] = i;
    }
    
    for (int i=0; i<(2*n); i++) {
        int a = random()%n;
        int b = random()%n;
        int tmp = data_sort_r[a];
        data_sort_r[a] = data_sort_r[b];
        data_sort_r[b] = tmp;
    }

    printf("Testing %i elements: ", n);
    sort_r(data_sort_r, n, sizeof(int), compfunc, g_userarg);
    for (int i=0; i<n; i++) {
        //printf("%i\t%i\t%i\n", i, data_sorted[i], data_sort_r[i]);
        assert(data_sorted[i] == data_sort_r[i]);
    }
    printf("compare: OK\n");

    free(data_sort_r);
    free(data_sorted);
}


int main(int argc, char *argv[])
{
    unsigned int seed = (unsigned int) time(NULL);
    srand(seed);

    printf("Testing sort_r with random seed %u\n", seed);
    for (int i=0; i<=20; i++) sequencetest(i);
    sequencetest(100 + random() % 100);
    sequencetest(1000 + random() % 1000);
    sequencetest(10000 + random() % 10000);
    
    for (int i=0; i< 20; i++) randomtest(i);
    for (int i=20; i< 200; i+=10) randomtest(i);
    for (int i=200; i<= 1000; i+=100) randomtest(i);
    randomtest(1000 + random() % 1000);
    randomtest(10000 + random() % 10000);
    randomtest(100000 + random() % 100000);
    randomtest(1000000 + random() % 1000000);

    return 0;
}
