//#include <tk/tkernel.h>
#include <stdint.h>
#include <time.h>

#include "common_utils.h"
#include "monotonic_timer.h"

int clock_gettime(clockid_t clk_id, struct timespec *tp)
{
    uint64_t usec = g_mono_usec;

    tp->tv_sec  = usec / 1000000ULL;
    tp->tv_nsec = (usec % 1000000ULL) * 1000ULL;

    return 0;
}
