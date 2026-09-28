#ifndef CAMERA_TASK_H
#define CAMERA_TASK_H

#include "common_utils.h"
#include "task_common_utils.h"
#include "board_i2c_master.h"

extern void camera_task(INT stacd, void *exinf);
extern ID     camera_tskid;
extern T_CTSK camera_ctsk;

extern ID camera_flgid;
extern ID init_flgid;

#endif /* CAMERA_TASK_H */
