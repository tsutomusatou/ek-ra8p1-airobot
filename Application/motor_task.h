#ifndef MOTOR_TASK_H
#define MOTOR_TASK_H

#include "task_common_utils.h"

extern void motor_task(INT stacd, void *exinf);

extern ID     motor_tskid;
extern T_CTSK motor_ctsk;

extern ID init_flgid;

#endif /* MOTOR_TASK_H */
