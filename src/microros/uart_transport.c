#include <unistd.h>

#include "microros_transports.h"
#include "common_utils.h"

#include <rcl/rcl.h>
#include <rclc/rclc.h>

#ifndef RMW_UXRCE_TRANSPORT_CUSTOM

// --- micro-ROS Transports ---
#define UART_RX_BUF_SIZE 4096
uint8_t it_buffer[1];
static uint8_t rx_ring_buf[UART_RX_BUF_SIZE];
static volatile uint16_t rx_w = 0;
static volatile uint16_t rx_r = 0;

/* UART RX Event Flag */
ID flg_uart_rx;
uint8_t FLG_RX_DONE = 0x01;

ID flg_uart_tx;
uint8_t FLG_TX_DONE = 0x01;

bool g_write_complete = false;
//bool g_read_complete = false;

//static size_t it_head = 0, it_tail = 0;

/* Add 1 byte to ring buffer */
static inline void ring_push(uint8_t c)
{
    uint16_t next = (rx_w + 1) % UART_RX_BUF_SIZE;
    if (next != rx_r) {
        rx_ring_buf[rx_w] = c;
        rx_w = next;
    }
}

/* Get 1 byte from ring buffer */
inline bool ring_pop(uint8_t *out)
{
    if (rx_r == rx_w) return false;
    *out = rx_ring_buf[rx_r];
    rx_r = (rx_r + 1) % UART_RX_BUF_SIZE;
    return true;
}

void uros_uart_callback (uart_callback_args_t *p_args)
{
//    SEGGER_RTT_printf(0, "uart event=%d\n", p_args->event);

    switch (p_args->event)
    {
        case UART_EVENT_ERR_OVERFLOW:
            SEGGER_RTT_printf(0, "Overflow occurred.\n");
            break;

        case UART_EVENT_TX_COMPLETE:
            tk_set_flg(flg_uart_tx, FLG_TX_DONE);
            g_write_complete = true;
            break;

        case UART_EVENT_RX_COMPLETE:
            ring_push(it_buffer[0]);                 // Put received bytes in ring buffer
            tk_set_flg(flg_uart_rx, FLG_RX_DONE);              // Wake up uros_uart_task
            R_SCI_B_UART_Read(&g_uart0_ctrl, &it_buffer[0], 1); /* Get ready for the next read */
            break;

//        case UART_EVENT_RX_COMPLETE:
//        {
//            for (uint32_t i = 0; i < UART_RX_BUF_SIZE; i++)
//            {
//                ring_push(it_buffer[i]);
//            }
//
//            tk_set_flg(flg_uart_rx, FLG_RX_DONE);
//
//            R_SCI_B_UART_Read(&g_uart0_ctrl, it_buffer, sizeof(it_buffer));
//            break;
//        }

        case UART_EVENT_RX_CHAR:
            SEGGER_RTT_printf(0, "RX: %c (0x%02X)\n", p_args->data, p_args->data);

            /* 次の1byte受信を開始 */
            R_SCI_B_UART_Read(&g_uart0_ctrl, &it_buffer[0], 1);
            break;

        default:
            break;
    }
}


//bool renesas_e2_transport_open(void){
bool uros_uart_open(void){
    fsp_err_t err;

    R_SCI_B_UART_Open(&g_uart0_ctrl, &g_uart0_cfg);
    err = R_SCI_B_UART_Read(&g_uart0_ctrl, &it_buffer[0], 1);

    return err == FSP_SUCCESS;
}

bool uart_send_byte(uint8_t c)
{
    g_write_complete = false;

    fsp_err_t err = R_SCI_B_UART_Write(&g_uart0_ctrl, &c, 1);
    if (FSP_SUCCESS != err)
    {
        return false;
    }

    while (!g_write_complete)
    {
        /* busy wait or use event flag */
    }

    return true;
}

size_t uart_ringbuffer_read(uint8_t *buf, size_t len, int timeout_ms)
{
    size_t read_count = 0;
    SYSTIM start, now;
    tk_get_tim(&start);

    while (read_count < len)
    {
        /* バッファにデータがある？ */
        if (rx_r != rx_w)
        {
            buf[read_count++] = rx_ring_buf[rx_r];
            rx_r = (rx_r + 1) % UART_RX_BUF_SIZE;
        }
        else
        {
            /* データが来るまでイベント待ち */
            UINT flg;
            ER ercd = tk_wai_flg(flg_uart_rx, FLG_RX_DONE, TWF_ORW, &flg, timeout_ms);
            if (ercd != E_OK)
            {
                break;  // timeout
            }
        }

        tk_get_tim(&now);
        if ((now.lo - start.lo) >= timeout_ms)
        {
            break;
        }
    }

    return read_count;
}


bool my_open_transport(struct uxrCustomTransport * transport)
{
    return true;
}

bool my_close_transport(struct uxrCustomTransport * transport)
{
    return true;
}

size_t my_write_transport(struct uxrCustomTransport* transport, const uint8_t * buf, size_t len, uint8_t * error)
{
    g_write_complete = false;

    fsp_err_t err = R_SCI_B_UART_Write(&g_uart0_ctrl, buf, len);

    if (FSP_SUCCESS != err)
    {
        return 0;
    }

    // ★ 送信完了を待つ
    while (!g_write_complete) {
        // μT-Kernel なら tslp_tsk(1) などで軽く待つ
        tk_dly_tsk(1);
    }

    return len;
}

size_t my_read_transport(struct uxrCustomTransport* transport,
                         uint8_t* buf,
                         size_t len,
                         int timeout,
                         uint8_t* err)
{
    size_t read_count = 0;
    SYSTIM start, now;
    tk_get_tim(&start);


//    SEGGER_RTT_printf(0,"read len=%d timeout=%d\n", len, timeout);

    while (read_count < len)
    {
        if (rx_r != rx_w)
        {
            buf[read_count++] = rx_ring_buf[rx_r];
            rx_r = (rx_r + 1) % UART_RX_BUF_SIZE;
        }
        else
        {
            tk_get_tim(&now);
            if ((now.lo - start.lo) >= timeout)
            {
//                SEGGER_RTT_printf(0,"read timeout.\n");
                break;
            }
        }
    }

    return read_count;
}

#endif //RMW_UXRCE_TRANSPORT_CUSTOM
