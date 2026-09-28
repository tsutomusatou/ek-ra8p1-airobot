#include "sensor_task.h"
#include "task_message.h"

ID     sensor_tskid;
T_CTSK sensor_ctsk = {
    .itskpri    = 5,
    .stksz      = 2048,
    .task       = sensor_task,
    .tskatr     = TA_HLNG | TA_RNG0,
};

static T_SENSOR_MSG sensor_msg;

void sensor_task(INT stacd, void *exinf)
{
    (void)stacd;
    (void)exinf;

    uint16_t distance = 0;

    UINT flgptn;
    tk_wai_flg(init_flgid, FLAG_CONTROL_INIT_COMPLETION, TWF_ORW, &flgptn, TMO_FEVR);

    APP_PRINT("[TOF] Start task.\n");


    while (1) {
        distance = tof_read_distance();

        sensor_msg.tof_value = distance;

        /* sensor_task -> control_task */
//        APP_PRINT("[TOF] sensor >> control : %d\n", distance);
        tk_snd_mbx(MBX_SENSOR_TO_CONTROL, (T_MSG*)&sensor_msg);

        tk_dly_tsk(100); // 100ms cycle
    }
}
