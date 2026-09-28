#include "motor_task.h"
#include "task_message.h"

typedef enum
{
    MOTOR_STATE_STOP,
    MOTOR_STATE_FORWARD,
    MOTOR_STATE_LEFT,
    MOTOR_STATE_RIGHT
} motor_state_t;

static motor_state_t motor_state = MOTOR_STATE_STOP;

ID     motor_tskid;
T_CTSK motor_ctsk = {
    .itskpri    = 9,
    .stksz      = 2048,
    .task       = motor_task,
    .tskatr     = TA_HLNG | TA_RNG0,
};

void motor_task(INT stacd, void *exinf)
{
    (void)stacd;
    (void)exinf;

    /* Wait for micro-ROS init completion */
    UINT flgptn;
    tk_wai_flg(init_flgid, FLAG_UROS_INIT_COMPLETION, TWF_ORW, &flgptn, TMO_FEVR);

    APP_PRINT("[MOT] Start task.\n");
    tk_set_flg(init_flgid, FLAG_MOTOR_INIT_COMPLETION);

    uint32_t duty_left  = 0;
    uint32_t duty_right = 0;


    motor_set_direction(false, false);
    motor_run(duty_left, duty_right);

    while (1) {
        /* Receive control message */
        T_CONTROL_CMD_MSG *control_msg;

        ER ercd = tk_rcv_mbx(MBX_CONTROL_TO_MOTOR, (T_MSG**)&control_msg, TMO_FEVR);
        if (ercd == E_OK) { // get a message from control.
//            APP_PRINT("[MOT] motor << control: %s\n", control_msg->text);

            if (strcmp(control_msg->text, "STOP") == 0)
            {
                if (motor_state != MOTOR_STATE_STOP)
                {
                    duty_left  = 0;
                    duty_right = 0;

                    motor_set_direction(false, false);

                    motor_state = MOTOR_STATE_STOP;

                    motor_run(duty_left, duty_right);
                }
            }
            else if (strcmp(control_msg->text, "RUN") == 0)
            {
                if (motor_state != MOTOR_STATE_FORWARD)
                {
                    duty_left  = 50;
                    duty_right = 50;

                    motor_set_direction(false, false);

                    motor_state = MOTOR_STATE_FORWARD;

                    motor_run(duty_left, duty_right);
                }
            }
            else if (strcmp(control_msg->text, "LEFT") == 0)
            {
                if (motor_state != MOTOR_STATE_LEFT)
                {
                    duty_left  = 50;
                    duty_right = 50;

                    motor_set_direction(true, false);

                    motor_state = MOTOR_STATE_LEFT;

                    motor_run(duty_left, duty_right);
                }
            }
            else if (strcmp(control_msg->text, "RIGHT") == 0)
            {
                if (motor_state != MOTOR_STATE_RIGHT)
                {
                    duty_left  = 50;
                    duty_right = 50;

                    motor_set_direction(false, true);

                    motor_state = MOTOR_STATE_RIGHT;

                    motor_run(duty_left, duty_right);
                }
            }
        }

    }
}
