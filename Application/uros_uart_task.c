#include "uros_uart_task.h"

ID     uros_uart_tskid;
T_CTSK uros_uart_ctsk = {
    .itskpri    = 10,
    .stksz      = 2048,
    .task       = uros_uart_task,
    .tskatr     = TA_HLNG | TA_RNG3,
};

void uros_uart_task(INT stacd, void *exinf)
{
    (void)stacd;
    (void)exinf;

    UINT flg;
    uint8_t c;

    while (1)
    {
        tk_wai_flg(flg_uart_rx, FLG_RX_DONE, TWF_ORW, &flg, TMO_FEVR);   // Wait for notification from ISR

        while (ring_pop(&c))                         // Get from ring buffer
        {
            SEGGER_RTT_printf(0, "RX: %c (0x%02X)\n", c, c);

            /* Echo back if necessary */
            uart_send_byte(c);
        }
    }
}

