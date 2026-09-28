#include "vin_camera.h"
#include "camera_task.h"
#include "display_task.h"

uint8_t * g_camera_frame_buffer = NULL;
volatile bool g_camera_frame_ready = false;

void vin0_callback (capture_callback_args_t * p_args)
{
    vin_event_t            event            = (vin_event_t) p_args->event;
    vin_module_status_t    module_status    = (vin_module_status_t) p_args->event_status;
    vin_interrupt_status_t interrupt_status = (vin_interrupt_status_t) p_args->interrupt_status;
    FSP_PARAMETER_NOT_USED(module_status);

    switch (event)
    {
        case VIN_EVENT_NOTIFY:
        {
            if (interrupt_status.bits.frame_complete)
            {
                g_camera_frame_buffer = p_args->p_buffer;
                g_camera_frame_ready = true;

                /* Tell display thread camera image has updated */
                display_next_buffer_set(p_args->p_buffer);

                /* Notify to camera task */
                tk_set_flg(camera_flgid, 0x01);
            }
            break;
        }

        case VIN_EVENT_ERROR:
        {
            // Process Error based on interrupt and module status;
            break;
        }

        default:
        {
            /* Do nothing */
            break;
        }
    }
}

void mipi_csi0_callback (mipi_csi_callback_args_t * p_args)
{
    APP_PRINT("MIPI CALLBACK event = %d\n", p_args->event);

    switch (p_args->event)
    {
        case MIPI_CSI_EVENT_DATA_LANE:
        {
            mipi_csi_data_lane_status_t data = p_args->event_data.data_lane_status;
            uint8_t lane_idx = p_args->event_idx;
            FSP_PARAMETER_NOT_USED(data);
            FSP_PARAMETER_NOT_USED(lane_idx);
            break;
        }

        case MIPI_CSI_EVENT_FRAME_DATA:
        {
            mipi_csi_data_lane_status_t data = p_args->event_data.data_lane_status;
            FSP_PARAMETER_NOT_USED(data);
            break;
        }

        case MIPI_CSI_EVENT_POWER:
        {
            mipi_csi_data_lane_status_t data = p_args->event_data.data_lane_status;
            FSP_PARAMETER_NOT_USED(data);
            break;
        }

        case MIPI_CSI_EVENT_SHORT_PACKET_FIFO:
        {
            mipi_csi_data_lane_status_t data = p_args->event_data.data_lane_status;
            FSP_PARAMETER_NOT_USED(data);
            break;
        }

        case MIPI_CSI_EVENT_VIRTUAL_CHANNEL:
        {
            mipi_csi_data_lane_status_t data = p_args->event_data.data_lane_status;
            uint8_t channel_idx = p_args->event_idx;
            FSP_PARAMETER_NOT_USED(channel_idx);
            if(data.bits.err_control || data.bits.err_escape)
            {
                ;
            }
            break;
        }

        default:
            break;
    }
}
void glcdc_vsync_isr(display_callback_args_t *p_args)
{

//    APP_PRINT("GLCDC VSYNC ISR CALLBACK event = %d\n", p_args->event);
#if 0
    FSP_PARAMETER_NOT_USED(p_args);

    g_update_fps_text = 1;

    switch (p_args->event)
    {
        case DISPLAY_EVENT_GR1_UNDERFLOW:
            __NOP();
            break;

        case DISPLAY_EVENT_GR2_UNDERFLOW:
            __NOP();
            break;

        default:
    }

    g_vsync_flag = 1;
#endif
    return;
}
