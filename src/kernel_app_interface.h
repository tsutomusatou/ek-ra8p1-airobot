#ifndef KERNEL_APP_INTERFACE_H
#define KERNEL_APP_INTERFACE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <tk/tkernel.h>
//#include <tm/tmonitor.h>
#include "uxr/client/transport.h"

//#include "hal_data.h"

extern bool uart_send_byte(uint8_t c);
extern bool ring_pop(uint8_t *out);

extern bool my_open_transport(struct uxrCustomTransport * transport);
extern bool my_close_transport(struct uxrCustomTransport * transport);
extern size_t my_write_transport(struct uxrCustomTransport* transport, const uint8_t * buf, size_t len, uint8_t * error);
extern size_t my_read_transport(struct uxrCustomTransport* transport, uint8_t* buf, size_t len, int timeout, uint8_t* err);

extern ID flg_uart_rx;
extern uint8_t FLG_RX_DONE;
extern bool g_write_complete;
//extern bool g_read_complete;

extern uint16_t tof_read_distance(void);

extern void motor_set_direction(bool left_reverse, bool right_reverse);
extern void motor_stop(void);
extern void motor_run(uint32_t duty_left_percent, uint32_t duty_right_percent);

#endif // KERNEL_APP_INTERFACE_H
