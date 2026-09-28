/* generated vector source file - do not edit */
#include "bsp_api.h"
/* Do not build these data structures if no interrupts are currently allocated because IAR will have build errors. */
#if VECTOR_DATA_IRQ_COUNT > 0
        BSP_DONT_REMOVE const fsp_vector_t g_vector_table[BSP_ICU_VECTOR_NUM_ENTRIES] BSP_PLACE_IN_SECTION(BSP_SECTION_APPLICATION_VECTORS) =
        {
                        [0] = glcdc_line_detect_isr, /* GLCDC LINE DETECT (Specified line) */
            [1] = glcdc_underflow_1_isr, /* GLCDC UNDERFLOW 1 (Graphic 1 underflow) */
            [2] = glcdc_underflow_2_isr, /* GLCDC UNDERFLOW 2 (Graphic 2 underflow) */
            [3] = iic_master_rxi_isr, /* IIC1 RXI (Receive data full) */
            [4] = iic_master_txi_isr, /* IIC1 TXI (Transmit data empty) */
            [5] = iic_master_tei_isr, /* IIC1 TEI (Transmit end) */
            [6] = iic_master_eri_isr, /* IIC1 ERI (Transfer error) */
            [7] = vin_status_isr, /* VIN IRQ (Interrupt Request) */
            [8] = vin_error_isr, /* VIN ERR (Interrupt Request for SYNC Error) */
            [9] = mipi_csi_rx_isr, /* MIPICSI RX (Receive interrupt) */
            [10] = mipi_csi_dl_isr, /* MIPICSI DL (Data Lane interrupt) */
            [11] = mipi_csi_vc_isr, /* MIPICSI VC (Virtual Channel interrupt) */
            [12] = mipi_csi_pm_isr, /* MIPICSI PM (Power Management interrupt) */
            [13] = mipi_csi_gst_isr, /* MIPICSI GST (Generic Short Packet interrupt) */
            [14] = rm_ethosu_isr, /* NPU IRQ (NPU IRQ) */
            [15] = sci_b_uart_rxi_isr, /* SCI0 RXI (Receive data full) */
            [16] = sci_b_uart_txi_isr, /* SCI0 TXI (Transmit data empty) */
            [17] = sci_b_uart_tei_isr, /* SCI0 TEI (Transmit end) */
            [18] = sci_b_uart_eri_isr, /* SCI0 ERI (Receive error) */
            [19] = iic_master_rxi_isr, /* IIC0 RXI (Receive data full) */
            [20] = iic_master_txi_isr, /* IIC0 TXI (Transmit data empty) */
            [21] = iic_master_tei_isr, /* IIC0 TEI (Transmit end) */
            [22] = iic_master_eri_isr, /* IIC0 ERI (Transfer error) */
            [23] = agt_int_isr, /* AGT0 INT (AGT interrupt) */
            [24] = r_icu_isr, /* ICU IRQ0 (External pin interrupt 0) */
            [25] = r_icu_isr, /* ICU IRQ3 (External pin interrupt 3) */
        };
        #if BSP_FEATURE_ICU_HAS_IELSR
        const bsp_interrupt_event_t g_interrupt_event_link_select[BSP_ICU_VECTOR_NUM_ENTRIES] =
        {
            [0] = BSP_PRV_VECT_ENUM(EVENT_GLCDC_LINE_DETECT,GROUP0), /* GLCDC LINE DETECT (Specified line) */
            [1] = BSP_PRV_VECT_ENUM(EVENT_GLCDC_UNDERFLOW_1,GROUP1), /* GLCDC UNDERFLOW 1 (Graphic 1 underflow) */
            [2] = BSP_PRV_VECT_ENUM(EVENT_GLCDC_UNDERFLOW_2,GROUP2), /* GLCDC UNDERFLOW 2 (Graphic 2 underflow) */
            [3] = BSP_PRV_VECT_ENUM(EVENT_IIC1_RXI,GROUP3), /* IIC1 RXI (Receive data full) */
            [4] = BSP_PRV_VECT_ENUM(EVENT_IIC1_TXI,GROUP4), /* IIC1 TXI (Transmit data empty) */
            [5] = BSP_PRV_VECT_ENUM(EVENT_IIC1_TEI,GROUP5), /* IIC1 TEI (Transmit end) */
            [6] = BSP_PRV_VECT_ENUM(EVENT_IIC1_ERI,GROUP6), /* IIC1 ERI (Transfer error) */
            [7] = BSP_PRV_VECT_ENUM(EVENT_VIN_IRQ,GROUP7), /* VIN IRQ (Interrupt Request) */
            [8] = BSP_PRV_VECT_ENUM(EVENT_VIN_ERR,GROUP0), /* VIN ERR (Interrupt Request for SYNC Error) */
            [9] = BSP_PRV_VECT_ENUM(EVENT_MIPICSI_RX,GROUP1), /* MIPICSI RX (Receive interrupt) */
            [10] = BSP_PRV_VECT_ENUM(EVENT_MIPICSI_DL,GROUP2), /* MIPICSI DL (Data Lane interrupt) */
            [11] = BSP_PRV_VECT_ENUM(EVENT_MIPICSI_VC,GROUP3), /* MIPICSI VC (Virtual Channel interrupt) */
            [12] = BSP_PRV_VECT_ENUM(EVENT_MIPICSI_PM,GROUP4), /* MIPICSI PM (Power Management interrupt) */
            [13] = BSP_PRV_VECT_ENUM(EVENT_MIPICSI_GST,GROUP5), /* MIPICSI GST (Generic Short Packet interrupt) */
            [14] = BSP_PRV_VECT_ENUM(EVENT_NPU_IRQ,GROUP6), /* NPU IRQ (NPU IRQ) */
            [15] = BSP_PRV_VECT_ENUM(EVENT_SCI0_RXI,GROUP7), /* SCI0 RXI (Receive data full) */
            [16] = BSP_PRV_VECT_ENUM(EVENT_SCI0_TXI,GROUP0), /* SCI0 TXI (Transmit data empty) */
            [17] = BSP_PRV_VECT_ENUM(EVENT_SCI0_TEI,GROUP1), /* SCI0 TEI (Transmit end) */
            [18] = BSP_PRV_VECT_ENUM(EVENT_SCI0_ERI,GROUP2), /* SCI0 ERI (Receive error) */
            [19] = BSP_PRV_VECT_ENUM(EVENT_IIC0_RXI,GROUP3), /* IIC0 RXI (Receive data full) */
            [20] = BSP_PRV_VECT_ENUM(EVENT_IIC0_TXI,GROUP4), /* IIC0 TXI (Transmit data empty) */
            [21] = BSP_PRV_VECT_ENUM(EVENT_IIC0_TEI,GROUP5), /* IIC0 TEI (Transmit end) */
            [22] = BSP_PRV_VECT_ENUM(EVENT_IIC0_ERI,GROUP6), /* IIC0 ERI (Transfer error) */
            [23] = BSP_PRV_VECT_ENUM(EVENT_AGT0_INT,GROUP7), /* AGT0 INT (AGT interrupt) */
            [24] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ0,GROUP0), /* ICU IRQ0 (External pin interrupt 0) */
            [25] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ3,GROUP1), /* ICU IRQ3 (External pin interrupt 3) */
        };
        #endif
        #endif
