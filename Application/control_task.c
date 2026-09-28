#include "control_task.h"
#include "task_message.h"
#include "eirq_encoder.h"

#define ENABLE_HOST_CONTROL 0 // Motor can be controlled by remote host

ID     control_tskid;
T_CTSK control_ctsk = {
    .itskpri    = 7,
    .stksz      = 4096,
    .task       = control_task,
    .tskatr     = TA_HLNG | TA_RNG0,
};

static T_CONTROL_CMD_MSG control_msg;

#if ENABLE_ROS_HOST
static T_CONTROL_CMD_MSG m;
static bool ros_cmd_updated = false;
static bool stop_sent = false;
static SYSTIM last_ros_cmd_time, now;
#endif

#define TOF_AVG_WINDOW   5

static int tof_buf[TOF_AVG_WINDOW];
static int tof_index = 0;
static int tof_sum = 0;
static int tof_avg = 0;

/*------------------------------------------------*
 *  Define paths to goal                          *
 *------------------------------------------------*/
#define GOAL_SEAT_1  1
#define GOAL_SEAT_2  2
#define GOAL_SEAT_3  3
#define GOAL_SEAT_4  4

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

typedef enum
{
    CTRL_WAIT_DETECTION,   // Wait for object detection
    CTRL_WAIT_START,       // Wait for start
    CTRL_FORWARD,          // Move forward
    CTRL_TURN_LEFT,        // Turn left
    CTRL_TURN_RIGHT,       // Turn right
    CTRL_FINISH            // Reach goal
} control_state_t;

static control_state_t control_state = CTRL_WAIT_DETECTION;

typedef enum
{
    MOVE_FORWARD,
    TURN_LEFT,
    TURN_RIGHT,
    MOVE_STOP
} move_type_t;

typedef struct
{
    move_type_t type;
    int value;
} route_command_t;

// For checking motor movement
//static const route_command_t route_table1[] =
//{
//    { MOVE_FORWARD, 2000 },
//    { TURN_RIGHT,     90 },
//    { MOVE_FORWARD, 2000 },
//    { TURN_LEFT,     90 }
//};

static const route_command_t route_table1[] =
{
    { MOVE_FORWARD, 500 },
    { TURN_LEFT,     90 },
    { MOVE_FORWARD, 230 },
    { TURN_RIGHT,    90 },
    { MOVE_FORWARD, 300 }
};

static const route_command_t route_table2[] =
{
    { MOVE_FORWARD, 500 },
    { TURN_RIGHT,    90 },
    { MOVE_FORWARD,  90 },
    { TURN_LEFT,     90 },
    { MOVE_FORWARD, 300 }
};

static const route_command_t route_table3[] =
{
    { MOVE_FORWARD, 500 },
    { TURN_LEFT,     90 },
    { MOVE_FORWARD, 800 },
    { TURN_RIGHT,    90 },
    { MOVE_FORWARD, 800 },
    { TURN_RIGHT,    90 },
    { MOVE_FORWARD, 600 },
    { TURN_RIGHT,    90 },
    { MOVE_FORWARD, 300 }
};

static const route_command_t route_table4[] =
{
    { MOVE_FORWARD, 500 },
    { TURN_LEFT,     90 },
    { MOVE_FORWARD, 800 },
    { TURN_RIGHT,    90 },
    { MOVE_FORWARD, 800 },
    { TURN_RIGHT,    90 },
    { MOVE_FORWARD, 930 },
    { TURN_RIGHT,    90 },
    { MOVE_FORWARD, 300 }
};

static route_command_t *current_route;
static int current_route_count;
static int route_index = 0;

static int32_t start_encoder_left;
static int32_t start_encoder_right;

static void update_tof_average(int new_value)
{
    /* Subtract an old value */
    tof_sum -= tof_buf[tof_index];

    /* Add a new value */
    tof_buf[tof_index] = new_value;
    tof_sum += new_value;

    /* Move to the next position */
    tof_index = (tof_index + 1) % TOF_AVG_WINDOW;

    /* Calculate average */
    tof_avg = tof_sum / TOF_AVG_WINDOW;
}

static bool forward_distance_reached(int32_t target_mm)
{
    int32_t left_delta;
    int32_t right_delta;
    int32_t distance_count;

    APP_PRINT("[CTL] ENCODER value L=%d R=%d\n", encoder_left, encoder_right);

    left_delta =
        encoder_left - start_encoder_left;

    right_delta =
        encoder_right - start_encoder_right;

    /*
     * 左右の移動量の平均
     */
    distance_count =
        (left_delta + right_delta) / 2;

    APP_PRINT(
        "[CTL] FORWARD: L=%d R=%d AVG=%d\n",
        left_delta,
        right_delta,
        distance_count
    );

    /*
     * 現時点では
     * 1 count ≒ 10.2mm と仮定
     *
     * 実機測定後に変更する
     */
    int32_t target_count =
        target_mm / 10;

    return (distance_count >= target_count);
}

#define TURN_90_COUNT  20    /* 90 deg */
#define TURN_L_90_COUNT  22  /* 90 deg for left turn */

static bool turn_reached(int32_t angle)
{
    int32_t turn_count;
    int32_t left_delta;
    int32_t right_delta;

    APP_PRINT("[CTL] turn_reached: control_state = %d\n", control_state);

    left_delta  = encoder_left  - start_encoder_left;
    right_delta = encoder_right - start_encoder_right;

    if (angle == 90)
    {
        turn_count = TURN_90_COUNT;

        if (control_state == CTRL_TURN_LEFT) // Adjustment
        {
            turn_count = TURN_L_90_COUNT;
        }
    }
    else
    {
        return false; // 90 deg only is supported now
    }

    if (control_state == CTRL_TURN_LEFT)
    {
        left_delta = -left_delta;

        APP_PRINT(
            "[CTL] TURN LEFT: L=%d R=%d\n",
            left_delta,
            right_delta);

        return (left_delta <= -turn_count &&
                right_delta >= turn_count);
    }
    else if (control_state == CTRL_TURN_RIGHT)
    {
        right_delta = -right_delta;

        APP_PRINT(
            "[CTL] TURN RIGHT: L=%d R=%d\n",
            left_delta,
            right_delta);

        return (left_delta >= turn_count &&
                right_delta <= -turn_count);
    }

    return false;
}

bool start_button_pressed()
{
    // Get button status
    APP_PRINT("[CTL] START button pressed\n");

    return true; //TBD
}

static void advance_route(void)
{
    route_index++;

    start_encoder_left  = encoder_left;
    start_encoder_right = encoder_right;
}

static void update_control_state_from_route(void)
{
    if (route_index >= current_route_count)
    {
        control_state = CTRL_FINISH;
        return;
    }

    switch (current_route[route_index].type)
    {
    case MOVE_FORWARD:
        control_state = CTRL_FORWARD;
        break;

    case TURN_LEFT:
        control_state = CTRL_TURN_LEFT;
        break;

    case TURN_RIGHT:
        control_state = CTRL_TURN_RIGHT;
        break;

    default:
        control_state = CTRL_FINISH;
        break;
    }
}

void control_task(INT stacd, void *exinf)
{
    (void)stacd;
    (void)exinf;

    UINT flgptn;
    ER ercd;

#if ENABLE_ROS_HOST
    T_ROS_CMD_MSG *ros_msg;
#endif
    T_SENSOR_MSG *sensor_msg;
    T_CAMERA_MSG *camera_msg;

    /* Wait for start of task */
    tk_wai_flg(init_flgid, FLAG_MOTOR_INIT_COMPLETION, TWF_ORW, &flgptn, TMO_FEVR);
    APP_PRINT("[CTL] Start task.\n");

    /* Initialize buffer to store ToF sensor value */
    for (int i = 0; i < TOF_AVG_WINDOW; i++) {
        tof_buf[i] = 0;
    }

    tk_set_flg(init_flgid, FLAG_CONTROL_INIT_COMPLETION);

#if ENABLE_ROS_HOST
    tk_get_tim(&last_ros_cmd_time);
#endif

    //TBD
    bool btn_state = true;

    /*--------------------------------------------------
                      MAIN LOOP
      --------------------------------------------------*/
    while (1) {
#if ENABLE_ROS_HOST
        /* uros_task -> control_task */
        ercd = tk_rcv_mbx(MBX_ROS_TO_CONTROL, (T_MSG**)&ros_msg, TMO_POL);
        if (ercd == E_OK) { // get a message from uros.
            APP_PRINT("[CTL] control << uros : %s\n", ros_msg->text);
            ros_cmd_updated = true;
            tk_get_tim(&last_ros_cmd_time);
            // NEED TO MOD: send the same message to motor.
            strncpy(control_msg.text, ros_msg->text, sizeof(control_msg.text) - 1);
        }

        /* Stop when ROS instruction does not come */
        tk_get_tim(&now);
        if (now.lo - last_ros_cmd_time.lo > 1500) {
            if(!stop_sent) {
                APP_PRINT("[CTL] No message from uros\n");
                strcpy(control_msg.text, "STOP");
                ros_cmd_updated = true;
                stop_sent = true;
            }
        } else {
            stop_sent = false;
        }

        if (ros_cmd_updated) {
#if ENABLE_HOST_CONTROL
            /* control_task -> motor_task */
            APP_PRINT("[CTL] control >> motor\n");
            tk_snd_mbx(MBX_CONTROL_TO_MOTOR, (T_MSG*)&control_msg);
#endif
            ros_cmd_updated = false;
        }
#endif

        /*  sensor_task -> control_task */
        ercd = tk_rcv_mbx(MBX_SENSOR_TO_CONTROL, (T_MSG**)&sensor_msg, TMO_POL);
        if (ercd == E_OK) {
//            APP_PRINT("[CTL] control << sensor: %s\n", sensor_msg->text);

#if ENABLE_ROS_HOST
            strncpy(m.text, sensor_msg->text, sizeof(m.text)-1);
            /* control_task -> uros_task */
            APP_PRINT("[CTL] control >> uros\n");
            tk_snd_mbx(MBX_CONTROL_TO_ROS, (T_MSG*)&m);
#endif

            uint16_t tof_value = sensor_msg->tof_value;
//            APP_PRINT("[CTL] TOF value = %d\n", tof_value);

            /* Update average */
            update_tof_average(tof_value);
            APP_PRINT("[CTL] TOF average = %d\n", tof_avg);
        }

        /*  camera_task -> control_task */
        ercd = tk_rcv_mbx(MBX_CAMERA_TO_CONTROL, (T_MSG**)&camera_msg, TMO_POL);
        if (ercd == E_OK) {
            if (control_state == CTRL_WAIT_DETECTION)
            {
                APP_PRINT("[CTL] Camera detected Seat ID = %d\n",
                                  camera_msg->seat_id);

                switch (camera_msg->seat_id)
                {
                    case 1:
                        current_route = route_table1;
                        current_route_count =
                            sizeof(route_table1) / sizeof(route_table1[0]);
                        break;

                    case 2:
                        current_route = route_table2;
                        current_route_count =
                            sizeof(route_table2) / sizeof(route_table2[0]);
                        break;

                    case 3:
                        current_route = route_table3;
                        current_route_count =
                            sizeof(route_table3) / sizeof(route_table3[0]);
                        break;

                    case 4:
                        current_route = route_table4;
                        current_route_count =
                            sizeof(route_table4) / sizeof(route_table4[0]);
                        break;

                    default:
                        APP_PRINT("[CTL] Invalid Seat ID = %d\n",
                                  camera_msg->seat_id);
                        break;
                }

                if (camera_msg->seat_id >= 1 &&
                    camera_msg->seat_id <= 4)
                {
                    APP_PRINT("[CTL] Route was decided\n");

                    control_state = CTRL_WAIT_START;
                }
            }
        }

        switch (control_state)
        {
        case CTRL_WAIT_DETECTION:
            strncpy(control_msg.text, "STOP", sizeof(control_msg.text)-1);
            tk_dly_tsk(1000);
            break;
        case CTRL_WAIT_START:
//            APP_PRINT("[CTL] Wait for decision of route\n");
            /* STOP */
            strncpy(control_msg.text, "STOP", sizeof(control_msg.text)-1);

//                if (start_button_pressed())
            if(btn_state) // Ignore button state for now
            {
                APP_PRINT("[CTL] Start button pressed\n");

                btn_state = false; // TBD

                route_index = 0;

                start_encoder_left  = encoder_left;
                start_encoder_right = encoder_right;

                control_state = CTRL_FORWARD; // move forward for now
            }
            break;

        case CTRL_FORWARD:
            if (tof_avg < 200)
            {
                /* STOP */
                strncpy(control_msg.text, "STOP", sizeof(control_msg.text)-1);
            }
            else if(forward_distance_reached(current_route[route_index].value))
            {
                APP_PRINT("[CTL] FORWARD reached\n");
                /* STOP */
                strncpy(control_msg.text, "STOP", sizeof(control_msg.text)-1);

                advance_route();

                update_control_state_from_route();
            }
            else
            {
                /* Move forward */
                strncpy(control_msg.text, "RUN", sizeof(control_msg.text)-1);
            }
            break;

        case CTRL_TURN_LEFT:
            if (turn_reached(current_route[route_index].value))
            {
                APP_PRINT("[CTL] TURN LEFT reached\n");
                /* STOP */
                strncpy(control_msg.text, "STOP", sizeof(control_msg.text)-1);

                advance_route();

                update_control_state_from_route();
            }
            else
            {
                strncpy(control_msg.text, "LEFT", sizeof(control_msg.text)-1);
            }
            break;

        case CTRL_TURN_RIGHT:
            if (turn_reached(current_route[route_index].value))
            {
                APP_PRINT("[CTL] TURN RIGHT reached\n");
                /* STOP */
                strncpy(control_msg.text, "STOP", sizeof(control_msg.text)-1);

                advance_route();

                update_control_state_from_route();
            }
            else
            {
                strncpy(control_msg.text, "RIGHT", sizeof(control_msg.text)-1);
            }
            break;

        case CTRL_FINISH:
            APP_PRINT("[CTL] ROUTE FINISHED\n");
            /* STOP */
            strncpy(control_msg.text, "STOP", sizeof(control_msg.text)-1);

//            control_state = CTRL_WAIT_DETECTION;
            break;
        default:
            /* STOP */
            strncpy(control_msg.text, "STOP", sizeof(control_msg.text)-1);
            control_state = CTRL_WAIT_DETECTION;
            break;

        }

        /* control_task -> motor_task */
        APP_PRINT("[CTL] control >> motor : %s\n", control_msg.text);
        tk_snd_mbx(MBX_CONTROL_TO_MOTOR, (T_MSG*)&control_msg);

        tk_dly_tsk(1);
    }
}
