#ifndef UROS_TASK_H
#define UROS_TASK_H

#include "task_common_utils.h"

extern void uros_task(INT stacd, void *exinf);

extern ID     uros_tskid;
extern T_CTSK uros_ctsk;

extern ID init_flgid;

#endif /* UROS_TASK_H */
