#ifndef TASK_COMMON_UTILS_H_
#define TASK_COMMON_UTILS_H_

#include "SEGGER_RTT/SEGGER_RTT.h"

#include <tk/tkernel.h>
//#include <tm/tmonitor.h>

#include "hal_data.h"
#include "common_utils.h"
#include "kernel_app_interface.h"

#define FLAG_UROS_INIT_COMPLETION    0x01
#define FLAG_MOTOR_INIT_COMPLETION   0x02
#define FLAG_CONTROL_INIT_COMPLETION 0x04

#endif /* TASK_COMMON_UTILS_H_ */
