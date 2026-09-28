#include "common_utils.h"
//#include "kernel_app_interface.h"
#include "hal_data.h"
#include "pwm_motor.h"
#include "kernel_app_interface.h"

void pwm_timer_init(void) {
    fsp_err_t err;

    /*
     *  Open and start GPT1 for left motor
     */
    err = g_timer1.p_api->open(g_timer1.p_ctrl, g_timer1.p_cfg);
    APP_PRINT("[MOT][PWM] timer1 open=%d\n", err);

    APP_PRINT("[MOT][PWM] period_counts1=%d\n", g_timer1.p_cfg->period_counts);
//    uint32_t duty = g_timer1.p_cfg->period_counts / 2;

    g_timer1.p_api->dutyCycleSet(
        g_timer1.p_ctrl,
        0, // duty=0 : stop
        GPT_IO_PIN_GTIOCA
    );

    err = g_timer1.p_api->start(g_timer1.p_ctrl);
    APP_PRINT("[MOT][PWM] timer1 start=%d\n", err);

    /*
     *  Open and start GPT2 for right motor
     */
    err = g_timer2.p_api->open(g_timer2.p_ctrl, g_timer2.p_cfg);
    APP_PRINT("[MOT][PWM] timer2 open=%d\n", err);

//    APP_PRINT("[MOT][PWM] period_counts2=%d\n", g_timer2.p_cfg->period_counts);
//    duty = g_timer2.p_cfg->period_counts / 2;

    g_timer2.p_api->dutyCycleSet(
        g_timer2.p_ctrl,
        0, // duty=0 : stop
        GPT_IO_PIN_GTIOCA
    );

    err = g_timer2.p_api->start(g_timer2.p_ctrl);
    APP_PRINT("[MOT][PWM] timer2 start=%d\n", err);
}

void motor_set_direction(bool left_reverse, bool right_reverse)
{
    R_BSP_PinAccessEnable();

    /* Left motor */
    R_IOPORT_PinWrite(
        g_ioport.p_ctrl,
        BSP_IO_PORT_00_PIN_11,
        left_reverse ? BSP_IO_LEVEL_HIGH : BSP_IO_LEVEL_LOW);

    /* Right motor */
    R_IOPORT_PinWrite(
        g_ioport.p_ctrl,
        BSP_IO_PORT_00_PIN_14,
        right_reverse ? BSP_IO_LEVEL_HIGH : BSP_IO_LEVEL_LOW);

    R_BSP_PinAccessDisable();
}

void motor_run(uint32_t duty_left_percent, uint32_t duty_right_percent)
{
//    fsp_err_t err;

    APP_PRINT("[MOT][PWM] Duty left=%d right=%d\n", duty_left_percent, duty_right_percent);

    uint32_t duty_left  = g_timer1.p_cfg->period_counts * duty_left_percent  / 100;
    uint32_t duty_right = g_timer2.p_cfg->period_counts * duty_right_percent / 100;

    /* Left motor */
    g_timer1.p_api->dutyCycleSet(
        g_timer1.p_ctrl,
        duty_left,
        GPT_IO_PIN_GTIOCA
    );

    /* Right motor */
    g_timer2.p_api->dutyCycleSet(
        g_timer2.p_ctrl,
        duty_right,
        GPT_IO_PIN_GTIOCA
    );

}
