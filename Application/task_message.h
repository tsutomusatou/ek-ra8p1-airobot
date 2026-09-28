#ifndef TASK_MESSAGE_H
#define TASK_MESSAGE_H

#include <tk/tkernel.h>

/* ============================
 * メールボックス ID
 * ============================ */
extern ID MBX_SENSOR_TO_CONTROL;
extern ID MBX_ROS_TO_CONTROL;
extern ID MBX_CONTROL_TO_MOTOR;
extern ID MBX_CONTROL_TO_ROS;
extern ID MBX_CAMERA_TO_CONTROL;

/* ============================
 * メッセージ構造体
 * ============================ */
typedef struct {
    T_MSG msg;
    uint16_t tof_value;
} T_SENSOR_MSG;

typedef struct {
    T_MSG msg;
    char text[64];
} T_ROS_CMD_MSG;

typedef struct {
    T_MSG msg;
    char text[64];
} T_CONTROL_CMD_MSG;

typedef struct {
    T_MSG msg;
    char text[64];
} T_STATUS_MSG;

typedef struct
{
    T_MSG msg;
    int32_t seat_id;
} T_CAMERA_MSG;

extern void init_mailboxes(void);

#endif
