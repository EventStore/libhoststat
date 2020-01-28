#include <hoststat.h>
#include <stdlib.h>
#include <assert.h>

int get_host_load_averages(struct load_averages *load_averages) {
    assert(load_averages != NULL);

    const int num_avgs = 3;
    double avgs[num_avgs];

    int ret = getloadavg(avgs, num_avgs);
    if (ret < 0 || ret > 3) {
        return HOSTSTAT_ERROR;
    }

    load_averages->average_1m = avgs[0];
    load_averages->average_5m = avgs[1];
    load_averages->average_15m = avgs[2];
    return HOSTSTAT_SUCCESS;
}
