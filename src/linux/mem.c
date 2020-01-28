#include <hoststat.h>

#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Expands to the given string literal and the length of the string, not including
// the null terminator.
#define STR_WITH_LEN(s) s, (sizeof(s) -1)

int file_to_buffer(const char *path, char *buffer, int buf_len) {
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        return HOSTSTAT_UNAVAILABLE;
    }

    int len;
    int status;
    if ((len = read(fd, buffer, buf_len)) < 0) {
        status = HOSTSTAT_ERROR;
    } else {
        status = HOSTSTAT_SUCCESS;
        buffer[len] = '\0';
    }

    close(fd);

    return status;
}

static int meminfo_value(char *buffer, char *attr_name, int attr_name_len, uint64_t *result) {
    // Move the current pointer to the start of the attribute name
    char *current = strstr(buffer, attr_name);
    if (current == NULL) {
        return HOSTSTAT_UNAVAILABLE;
    }

    // Move to the whitespace after the attribute name, add 1 for the separator
    current += attr_name_len + 1;

    // Convert the value to a uint64_t. Note that scaling is not yet applied.
    int64_t value;
    char *value_end;
    value = strtoull(current, &value_end, 0);

    // Move to the unit specifier
    while (*value_end == ' ') {
        ++value_end;
    }

    // If there is a unit scale for kilobytes or megabytes, multiple the base
    // value accordingly, and set result.
    if (*value_end == 'k') {
        *result = value * 1024;
    } else if (*value_end == 'M') {
        *result = value * 1024 * 1024;
    }

    return HOSTSTAT_SUCCESS;
}

int get_meminfo_value(char *metric_name, int metric_name_len, uint64_t *value) {
    const int buf_size = 2048;
    char buffer[2048];

    int ret = file_to_buffer("/proc/meminfo", buffer, buf_size);
    if (ret != HOSTSTAT_SUCCESS) {
        return ret;
    }

    return meminfo_value(buffer, metric_name, metric_name_len, value);
}

int get_host_total_memory(uint64_t *total_memory) {
    return get_meminfo_value(STR_WITH_LEN("MemTotal"), total_memory);
}

int get_host_free_memory(uint64_t *free_memory) {
    return get_meminfo_value(STR_WITH_LEN("MemAvailable"), free_memory);
}
