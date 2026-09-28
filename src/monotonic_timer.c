#include "hal_data.h"
#include "common_utils.h"

volatile uint64_t g_mono_usec = 0;

void micro_ros_timer_cb(timer_callback_args_t *p_args)
{
    if (p_args->event == TIMER_EVENT_CYCLE_END)
    {
        g_mono_usec += 1000; // +1000 : msec --> usec
    }
}
