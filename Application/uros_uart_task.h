#ifndef UROS_UART_TASK_H
#define UROS_UART_TASK_H

#include "task_common_utils.h"

extern void uros_uart_task(INT stacd, void *exinf);

extern ID     uros_uart_tskid;
extern T_CTSK uros_uart_ctsk;

#endif /* UROS_UART_TASK_H */
