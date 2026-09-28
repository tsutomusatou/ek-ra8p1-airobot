#ifndef DISPLAY_TASK_H
#define DIAPLAY_TASK_H

#include "common_utils.h"
#include "task_common_utils.h"

extern void display_next_buffer_set(uint8_t* next_buffer);
extern void display_task(INT stacd, void *exinf);
extern ID     display_tskid;
extern T_CTSK display_ctsk;

extern ID g_i2c_flgid;
extern ID init_flgid;


#define PLCD_BLEN           (BSP_IO_PORT_05_PIN_14)

#endif /* DISPLAY_TASK_H */
