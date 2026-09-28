#ifndef SENSOR_TASK_H
#define SENSOR_TASK_H

#include "task_common_utils.h"

extern void sensor_task(INT stacd, void *exinf);

extern ID     sensor_tskid;
extern T_CTSK sensor_ctsk;

extern ID init_flgid;

#endif /* SENSOR_TASK_H */
