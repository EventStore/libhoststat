#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <stdlib.h>

#include <hoststat.h>

void test_get_host_total_memory() {
    uint64_t total_memory;
    int ret = get_host_total_memory(&total_memory);

    assert(ret == HOSTSTAT_SUCCESS);
    assert(total_memory > 0);

    printf("Host Total Memory: %" PRIu64 " bytes", total_memory);
}

void test_get_host_free_memory() {
    uint64_t free_memory;
    int ret = get_host_free_memory(&free_memory);
    assert(ret == HOSTSTAT_SUCCESS);
    assert(free_memory > 0);

    uint64_t total_memory;
    ret = get_host_total_memory(&total_memory);
    assert(ret == HOSTSTAT_SUCCESS);

    assert(free_memory < total_memory);

    printf("Free Memory: %" PRIu64 " bytes", free_memory);
}

void test_get_host_load_averages() {
    /**
     * Note this test sets (known) garbage values for the load averages since it seems that
     * the GitHub Actions runners _do_ occasionally have a load average of 0, especially at
     * the 15 minute interval..!
     */

    struct load_averages load_averages;
    load_averages.average_1m = -10;
    load_averages.average_5m = -10;
    load_averages.average_15m = -10;

    int ret = get_host_load_averages(&load_averages);
    assert(ret == HOSTSTAT_SUCCESS);

    assert(load_averages.average_1m >= 0);
    assert(load_averages.average_5m >= 0);
    assert(load_averages.average_15m >= 0);

    printf("Load Averages:\n\t1m:\t%.2f\n\t5m:\t%.2f\n\t15m:\t%.2f\n",
           load_averages.average_1m, load_averages.average_5m, load_averages.average_15m);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: hoststat_test <testname>");
        return 1;
    }

    if (strcmp(argv[1], "test_get_host_total_memory") == 0) {
        test_get_host_total_memory();
    }

    if (strcmp(argv[1], "test_get_host_free_memory") == 0) {
        test_get_host_free_memory();
    }

    if (strcmp(argv[1], "test_get_host_load_averages") == 0) {
        test_get_host_load_averages();
    }
}