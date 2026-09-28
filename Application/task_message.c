#include "task_message.h"

ID MBX_SENSOR_TO_CONTROL;
ID MBX_ROS_TO_CONTROL;
ID MBX_CONTROL_TO_MOTOR;
ID MBX_CONTROL_TO_ROS;
ID MBX_CAMERA_TO_CONTROL;

void init_mailboxes(void)
{
    T_CMBX cmbx = { .mbxatr = TA_TFIFO };

    MBX_SENSOR_TO_CONTROL = tk_cre_mbx(&cmbx);
    MBX_ROS_TO_CONTROL    = tk_cre_mbx(&cmbx);
    MBX_CONTROL_TO_MOTOR  = tk_cre_mbx(&cmbx);
    MBX_CONTROL_TO_ROS    = tk_cre_mbx(&cmbx);
    MBX_CAMERA_TO_CONTROL = tk_cre_mbx(&cmbx);
}
