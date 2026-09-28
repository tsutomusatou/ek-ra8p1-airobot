#ifndef CONTROL_TASK_H
#define CONTROL_TASK_H

#include "task_common_utils.h"

extern void control_task(INT stacd, void *exinf);

extern ID     control_tskid;
extern T_CTSK control_ctsk;

extern ID init_flgid;

#endif /* CONTROL_TASK_H */
