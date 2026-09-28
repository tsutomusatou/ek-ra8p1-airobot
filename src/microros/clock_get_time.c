#include <tk/tkernel.h>
#include "clock_get_time.h"

#include "common_utils.h"

uint64_t clock_get_time(void)
{
    SYSTIM tim;

    SEGGER_RTT_printf(0, "clock_get_time.\n");

    ER ercd = tk_get_tim(&tim);

    if (ercd < E_OK) {
        return 0;  // エラー時は 0 を返す
    }

    // 64bit に合成（μ秒単位）
    uint64_t usec = ((uint64_t)tim.hi << 32) | tim.lo;

    // micro-ROS が期待するナノ秒に変換
    return usec * 1000ULL;
}
