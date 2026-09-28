#include <tk/tkernel.h>
#include <tm/tmonitor.h>

#include "task_common_utils.h"

/* object detection */
#include "display_task.h"
#include "camera_task.h"

/* robot */
#include "uros_uart_task.h"
#include "uros_task.h"
#include "sensor_task.h"
#include "control_task.h"
#include "motor_task.h"
#include "task_message.h"

#define ENABLE_UART_DEBUG 0

ID g_i2c_flg_id;
ID init_flgid;

INT usermain(void);

static void create_sync_flag(void)
{
    T_CFLG cflg = {
        .flgatr = TA_TFIFO | TA_WMUL,
        .iflgptn = 0
    };
    init_flgid = tk_cre_flg(&cflg);

    APP_PRINT("Create init_flgid %d\n", init_flgid);
}


EXPORT INT usermain(void)
{
    T_CFLG cflg;
    ER err;

	APP_PRINT("=== Start User-main program ===\n");

    init_mailboxes();

	cflg.flgatr  = TA_WMUL | TA_TPRI;
	cflg.iflgptn = 0;
	cflg.exinf   = NULL;

	g_i2c_flg_id = tk_cre_flg(&cflg);
	if (g_i2c_flg_id < 0) {
	    APP_PRINT("Error: Failed to create I2C event flag.\n");
	    while(1);
	}

    create_sync_flag();

    /* Create & Start Tasks */
#if ENABLE_UART_DEBUG
    // Initial debug with UART
    uros_uart_tskid = tk_cre_tsk(&uros_uart_ctsk);
    tk_sta_tsk(uros_uart_tskid, 0);
#endif

    APP_PRINT("Create uROS task\n");
    uros_tskid = tk_cre_tsk(&uros_ctsk);
    tk_sta_tsk(uros_tskid, 0);

    APP_PRINT("Create Motor task\n");
    motor_tskid = tk_cre_tsk(&motor_ctsk);
    tk_sta_tsk(motor_tskid, 0);

    APP_PRINT("Create Control task\n");
    control_tskid = tk_cre_tsk(&control_ctsk);
    tk_sta_tsk(control_tskid, 0);

    APP_PRINT("Create Sensor(ToF) task\n");
    sensor_tskid = tk_cre_tsk(&sensor_ctsk);
    tk_sta_tsk(sensor_tskid, 0);

    APP_PRINT("Create display task\n");
	display_tskid = tk_cre_tsk(&display_ctsk);
	tk_sta_tsk(display_tskid, 0);

	APP_PRINT("Create camera task\n");
	camera_tskid = tk_cre_tsk(&camera_ctsk);
	tk_sta_tsk(camera_tskid, 0);

	tk_slp_tsk(TMO_FEVR);

	return 0;
}
