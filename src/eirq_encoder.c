#include "common_utils.h"
//#include "kernel_app_interface.h"
#include "hal_data.h"
#include "eirq_encoder.h"

volatile int32_t encoder_left  = 0;
volatile int32_t encoder_right = 0;

void encoder_left_callback(external_irq_callback_args_t *p_args)
{
    encoder_left++;
}

void encoder_right_callback(external_irq_callback_args_t *p_args)
{
    encoder_right++;
}

void encoder_irq_init(void)
{
    g_external_irq0.p_api->open(&g_external_irq0_ctrl, &g_external_irq0_cfg);
    g_external_irq0.p_api->enable(&g_external_irq0_ctrl);

    g_external_irq1.p_api->open(&g_external_irq1_ctrl, &g_external_irq1_cfg);
    g_external_irq1.p_api->enable(&g_external_irq1_ctrl);
}
