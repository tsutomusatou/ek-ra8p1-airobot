#include "std_msgs/msg/string.h"
#include "std_msgs/msg/int32.h"

#include "uros_task.h"
#include "task_message.h"

#include "rmw_microros/rmw_microros.h"   // rmw_uros_set_custom_transport()
//#include "rosidl_runtime_c/string_functions.h"
#include "rcl/rcl.h"
#include "rclc/rclc.h"
#include "rclc/executor.h"

#if ENABLE_ROS_HOST
static rclc_executor_t executor;
static rcl_allocator_t allocator;
static rcl_node_t node;
static rclc_support_t support;

static rcl_timer_t timer;

static rcl_publisher_t publisher;
static std_msgs__msg__String pub_msg;
//static std_msgs__msg__Int32 msg;

static rcl_subscription_t subscriber;
static std_msgs__msg__String sub_msg;
#endif

//ID MBX_ROS_TO_CONTROL;

//
//static void init_mailbox(void)
//{
//    T_CMBX cmbx = {
//        .mbxatr = TA_TFIFO   // ★ FIFO（必須）
//    };
//
//    MBX_ROS_TO_CONTROL = tk_cre_mbx(&cmbx);
//}

//static T_CONTROL_MSG control_msg;
//

ID     uros_tskid;
T_CTSK uros_ctsk = {
    .itskpri    = 12,
    .stksz      = 16384,
    .task       = uros_task,
    .tskatr     = TA_HLNG | TA_RNG0,
};

#if ENABLE_ROS_HOST
/* Host → MCU (control_task) */
void string_callback(const void * msgin)
{
    SEGGER_RTT_printf(0, "[Subscribed]\n");

    const std_msgs__msg__String * msg = (const std_msgs__msg__String *)msgin;

    static T_ROS_CMD_MSG control_msgs[2];
    static int idx = 0;

    T_ROS_CMD_MSG *m = &control_msgs[idx];
    idx ^= 1;

    /* ROS からの文字列をそのままコピー */
    strncpy(m->text, msg->data.data, sizeof(m->text) - 1);
    m->text[sizeof(m->text) - 1] = '\0';

    /* control_task へ渡す */
    SEGGER_RTT_printf(0, "uros >> control: %s\n", m->text);
    tk_snd_mbx(MBX_ROS_TO_CONTROL, (T_MSG*)m);


}
#endif

#if ENABLE_ROS_HOST
void timer_callback(rcl_timer_t * timer, int64_t last_call_time)
{
    (void) timer;
    (void) last_call_time;

    SEGGER_RTT_printf(0, "timer callback.\n");
}
#endif

void uros_task(INT stacd, void *exinf)
{
    (void)stacd;
    (void)exinf;

#if ENABLE_ROS_HOST
    rcl_ret_t rc;
    const int timeout_ms = 1000;
    const uint8_t attempts = 15;
#endif

    SEGGER_RTT_printf(0, "uros_task starts.\n");

#if ENABLE_ROS_HOST
    /* Register transport */
    rmw_uros_set_custom_transport(
//        false, // Packet starts with 80.
        true, // Framing mode. Packet starts with 7E.
        (void *)&g_uart0_ctrl,
//        NULL,
        my_open_transport,
        my_close_transport,
        my_write_transport,
        my_read_transport
    );

    rc = rmw_uros_ping_agent(timeout_ms, attempts);
    if (rc != RCL_RET_OK)
    {
        // Unreachable agent, exiting program.
        SEGGER_RTT_printf(0, "ERROR) Unreachable agent, exiting program.\n");
    } else {
        SEGGER_RTT_printf(0, "OK) Ping has reached the micro-ROS agent.\n");
    }

    /* Init micro-ROS */
    allocator = rcl_get_default_allocator();

    rc = rclc_support_init(&support, 0, NULL, &allocator);
    if (rc != RCL_RET_OK) {
        SEGGER_RTT_printf(0, "support_init error: %s\n", rcl_get_error_string().str);
        rcl_reset_error();
    }

    /* Create a node */
    SEGGER_RTT_printf(0, "create node.\n");

    rc = rclc_node_init_default(&node, "my_node", "", &support);
    if (rc != RCL_RET_OK) {
        SEGGER_RTT_printf(0, "ERROR) node_init: %s\n", rcl_get_error_string().str);
        rcl_reset_error();
    }

    rclc_publisher_init_default(
        &publisher,
        &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, String),
        "mcu_status"
    );

    std_msgs__msg__String__init(&pub_msg);
    pub_msg.data.data = malloc(128);
    pub_msg.data.capacity = 128;
    pub_msg.data.size = 0;

    rclc_subscription_init_default(
        &subscriber,
        &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, String),
        "cmd_string"
    );

    std_msgs__msg__String__init(&sub_msg);
    sub_msg.data.data = malloc(128);   // ★ 受信バッファを確保
    sub_msg.data.capacity = 128;
    sub_msg.data.size = 0;

    /* Create a timer */
    rc = rclc_timer_init_default(&timer, &support, RCL_MS_TO_NS(1000), timer_callback);
    SEGGER_RTT_printf(0, "timer_init rc=%d\n", rc);

    /*  Create an executor */
    rclc_executor_init(&executor, &support.context, 2, &allocator); // Timer and Subscriber


    /* Register the timer to  the executor */
    rc = rclc_executor_add_timer(&executor, &timer);
    SEGGER_RTT_printf(0, "add_timer rc=%d\n", rc);

    rclc_executor_add_subscription(
        &executor,
        &subscriber,
        &sub_msg,
        string_callback,
        ON_NEW_DATA
    );
#endif // End of ENABLE_ROS_HOST

    /* Notify init completion of motor task */
    tk_set_flg(init_flgid, FLAG_UROS_INIT_COMPLETION);

    /* Spin loop of executor */
    while (1) {
#if ENABLE_ROS_HOST
        rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100));

        T_SENSOR_MSG *sensor_msg;
        ER ercd = tk_rcv_mbx(MBX_CONTROL_TO_ROS, (T_MSG**)&sensor_msg, TMO_POL);
        if (ercd == E_OK) {
            SEGGER_RTT_printf(0, "uros << control : %s\n", sensor_msg->text);
            strncpy(pub_msg.data.data, sensor_msg->text, pub_msg.data.capacity - 1);
            pub_msg.data.size = strlen(pub_msg.data.data);

            SEGGER_RTT_printf(0, "ros >> Host : %s\n", pub_msg.data.data);
            rc = rcl_publish(&publisher, &pub_msg, NULL);
            if (rc != RCL_RET_OK) {
                SEGGER_RTT_printf(0,"[ros] publish failed: %d\n", rc);
            } else {
                SEGGER_RTT_printf(0, "[Published]\n");
            }
        }
#else
        tk_slp_tsk(TMO_FEVR);   // Sleep if uros is disabled.
#endif

    }
}

