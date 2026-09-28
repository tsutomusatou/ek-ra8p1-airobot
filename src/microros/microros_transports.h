#ifndef MICROROS_TRANSPORTS_H
#define MICROROS_TRANSPORTS_H

#include "hal_data.h"
#include "kernel_app_interface.h"

#define WRITE_TIMEOUT 100U

#ifdef NX_API_H
typedef struct custom_transport_args {
    ULONG agent_ip_address;
    UINT agent_port;
} custom_transport_args;
#elif defined(TCP_SOCKETS_WRAPPER_H)
typedef struct custom_transport_args {
    WIFINetworkParams_t * network_conf;
    const char agent_ip[16];
    uint16_t agent_port;
} custom_transport_args;
#endif

/* Tentative global definition for UART checking */
//#define UART_IT_BUFFER_SIZE 2048

//extern uint8_t it_buffer[UART_IT_BUFFER_SIZE];

//bool renesas_e2_transport_open(void);
bool uros_uart_open(void);

//bool renesas_e2_transport_open(struct uxrCustomTransport * transport);
//bool renesas_e2_transport_close(struct uxrCustomTransport * transport);
//size_t renesas_e2_transport_write(struct uxrCustomTransport* transport, const uint8_t * buf, size_t len, uint8_t * error);
//size_t renesas_e2_transport_read(struct uxrCustomTransport* transport, uint8_t* buf, size_t len, int timeout, uint8_t* err);

#endif  // MICROROS_TRANSPORTS_H
