#ifndef LIBHOSTSTAT_HOSTSTAT_H
#define LIBHOSTSTAT_HOSTSTAT_H

#include <stdint.h>

/**
 * Indicates a successful operation with a result that may be consumed.
 */
#define HOSTSTAT_SUCCESS 0

/**
 * Indicates an errored operation. The result may not be reliable.
 */
#define HOSTSTAT_ERROR 1

/**
 * Indicates a source of statistics is unavailable (for example, /proc being missing on Linux).
 */
#define HOSTSTAT_UNAVAILABLE 2

/**
 * Get the total hardware memory size of the host in bytes.
 *
 * @param total_memory - Pointer to a uint64_t for result
 * @return int - Return code. HOSTSTAT_SUCCESS if the result is usable,
 *               HOSTSTAT_ERROR if not.
 */
int get_host_total_memory(uint64_t *total_memory);

/**
 * Get the total instantaneous free memory of the host in bytes.
 *
 * @param free_memory - Pointer to a uint64_t for result
 * @return int - Return code. HOSTSTAT_SUCCESS if the result is usable,
 *               HOSTSTAT_ERROR if not.
 */
int get_host_free_memory(uint64_t *free_memory);

/**
 * A structure representing the 1, 5 and 15 minute load averages of the host.
 */
struct load_averages {
    double average_1m;
    double average_5m;
    double average_15m;
};

/**
 * Get the load averages of the host for 1, 5 and 15 minute intervals.
 */
int get_host_load_averages(struct load_averages *load_averages);

#endif //LIBHOSTSTAT_HOSTSTAT_H
