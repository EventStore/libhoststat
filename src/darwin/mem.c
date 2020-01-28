#include <hoststat.h>

#include <assert.h>
#include <mach/mach_init.h>
#include <mach/mach_host.h>
#include <sys/sysctl.h>

int get_host_total_memory(uint64_t *total_memory) {
    assert(total_memory != NULL);

    uint64_t hw_memsize;
    size_t hw_memsize_len = sizeof(hw_memsize);

    const int name_len = 2;
    int name[name_len] = {CTL_HW, HW_MEMSIZE};

    if (sysctl(name, name_len, &hw_memsize, &hw_memsize_len, NULL, 0) < 0) {
        return HOSTSTAT_ERROR;
    }

    *total_memory = hw_memsize;
    return HOSTSTAT_SUCCESS;
}

/**
 * Get the total instantaneous free memory of the host in bytes.
 *
 * @param free_memory - Pointer to a uint64_t for result
 * @return int - Return code. HOSTSTAT_SUCCESS if the result is usable,
 *               HOSTSTAT_ERROR if not.
 */
int get_host_free_memory(uint64_t *free_memory) {
    assert(free_memory != 0);

    mach_port_t mach_port = mach_host_self();

    vm_size_t page_size;
    if (host_page_size(mach_port, &page_size) != KERN_SUCCESS) {
        return HOSTSTAT_ERROR;
    }

    vm_statistics64_data_t vm_stats;
    mach_msg_type_number_t count = sizeof(vm_statistics64_data_t) / sizeof(natural_t);

    if (host_statistics64(mach_port, HOST_VM_INFO, (host_info64_t) &vm_stats, &count) != KERN_SUCCESS) {
        return HOSTSTAT_ERROR;
    }

    *free_memory = vm_stats.free_count * page_size;
    return HOSTSTAT_SUCCESS;
}
