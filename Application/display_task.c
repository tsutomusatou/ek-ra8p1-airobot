#include "display_task.h"
#include "camera_task.h"
#include "glcdc_ep.h"

/* Variables to store resolution information */
uint16_t g_hz_size, g_vr_size;
/* Variables used for buffer usage */
uint32_t g_buffer_size;
uint8_t * g_p_single_buffer, * g_p_double_buffer;
static uint8_t * gp_camera_buffer;

/* User defined functions */
#ifdef DISPLAY_IN_FORMAT_32BITS_RGB888_0
static void screen_display(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color);
#else
static void screen_display(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
#endif
static void color_band_display(void);

ID    display_tskid;          // Task ID number
T_CTSK display_ctsk = {               // Task creation information
    .itskpri    = 2,
    .stksz      = 16384,
    .task       = display_task,
    .tskatr     = TA_HLNG | TA_RNG3,
};

ID g_i2c_flgid;

void display_next_buffer_set(uint8_t* next_buffer)
{
    gp_camera_buffer = next_buffer;
}

/*******************************************************************************************************************//**
 * @brief       This function displays color bands on the screen using appropriate color codes and coordinate values.
 * @param[IN]   x1, y1  Start point coordinates.
 *              x2, y2  End point coordinates.
 *              color   Color to display.
 * @retval      None
 **********************************************************************************************************************/
#ifndef DISPLAY_IN_FORMAT_32BITS_RGB888_0
static void screen_display(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    /* Declare local variables */
    uint16_t start_x, start_y, display_length, display_height;
    uint32_t start_addr;

    /* Assign coordinate values and calculate start address */
    start_x = x1;
    start_y = y1;
    start_addr = (uint32_t)((start_x * BYTES_PER_PIXEL) + (start_y * g_hz_size* BYTES_PER_PIXEL));

    /* Calculate display box length and height */
    display_length = (uint16_t)((x2 - x1) * BYTES_PER_PIXEL);
    display_height = (y2 - y1);

    /* Display required color band */
    for(uint16_t ver_value = Y1_CO_ORDINATE; ver_value < (display_height - INC_DEC_VALUE); ver_value++)
    {
        for(uint32_t hor_value = start_addr; hor_value < (start_addr + display_length); hor_value += BYTES_PER_PIXEL)
        {
            *(uint16_t *) (g_p_single_buffer + hor_value) = color;
            *(uint16_t *) (g_p_double_buffer + hor_value) = color;
        }
        start_addr = (uint32_t)(start_addr + (g_hz_size * BYTES_PER_PIXEL));
    }
}
#else
static void screen_display(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color)
{
    /* Declare local variables */
    uint16_t start_x, start_y, display_length, display_height;
    uint32_t start_addr;

    /* Assign coordinate values and calculate start address */
    start_x = x1;
    start_y = y1;
    start_addr = (uint32_t)((start_x * BYTES_PER_PIXEL) + (start_y * g_hz_size* BYTES_PER_PIXEL));

    /* Calculate display box length and height */
    display_length = (uint16_t)((x2 - x1) * BYTES_PER_PIXEL);
    display_height = (y2 - y1);

    /* Display required color band */
    for(uint16_t ver_value = Y1_CO_ORDINATE; ver_value < (display_height - INC_DEC_VALUE); ver_value++)
    {
        for(uint32_t hor_value = start_addr; hor_value < (start_addr + display_length); hor_value += BYTES_PER_PIXEL)
        {
            *(uint32_t *) (g_p_single_buffer + hor_value) = color;
            *(uint32_t *) (g_p_double_buffer + hor_value) = color;
        }
        start_addr = (uint32_t)(start_addr + (g_hz_size * BYTES_PER_PIXEL));
    }
}
#endif

/*******************************************************************************************************************//**
 * @brief      This function displays eight horizontal color bands on the graphical LCD.
 * @param[IN]  None
 * @retval     None
 **********************************************************************************************************************/
static void color_band_display(void)
{
#ifdef DISPLAY_IN_FORMAT_32BITS_RGB888_0
    uint32_t color[COLOR_BAND_COUNT]= {RED, GREEN, BLUE, BLACK, WHITE, YELLOW, MAGENTA, CYAN};
#else
    uint16_t color[COLOR_BAND_COUNT]= {RED, GREEN, BLUE, BLACK, WHITE, YELLOW, MAGENTA, CYAN};
#endif
    uint16_t width = g_vr_size/COLOR_BAND_COUNT;

    for (uint8_t display_count = RESET_VALUE; display_count < COLOR_BAND_COUNT; display_count++)
    {
        screen_display((uint16_t)X1_CO_ORDINATE, (display_count * width), g_hz_size,\
                       (uint16_t)(((display_count * width) + width) + INC_DEC_VALUE), color[display_count]);
    }
}


/* タスク関数の本体 */
void display_task(INT stacd, void *exinf)
{
    (void)stacd;
    (void)exinf;

    fsp_err_t          err        = FSP_SUCCESS;
    UINT flgptn;

    T_CFLG cflg = {0};
    cflg.flgatr = TA_WSGL;
    cflg.iflgptn = 0;

    APP_PRINT("Start display task\n");

    g_i2c_flgid = tk_cre_flg(&cflg);
    if (g_i2c_flgid <= 0)
    {
        APP_PRINT("tk_cre_flg failed: %d\n", g_i2c_flgid);
    }

    /* Get LCDC configuration */
    g_hz_size = (g_plcd_display_cfg.input[0].hsize);
    g_vr_size = (g_plcd_display_cfg.input[0].vsize);
    /* Initialize buffer pointers */
    g_buffer_size = (uint32_t) (g_hz_size * g_vr_size * BYTES_PER_PIXEL);
    g_p_single_buffer = (uint8_t *) g_plcd_display_cfg.input[0].p_base;
    /* Double buffer for drawing color bands with good quality */
    g_p_double_buffer = g_p_single_buffer + g_buffer_size;

    R_BSP_PinAccessEnable();
    R_IOPORT_PinWrite(&g_ioport_ctrl, PLCD_BLEN, (bsp_io_level_t)BSP_IO_LEVEL_LOW);
    R_BSP_SoftwareDelay(40, BSP_DELAY_UNITS_MILLISECONDS);

    /* Initialize GLCDC driver */
    err = R_GLCDC_Open(&g_plcd_display_ctrl, &g_plcd_display_cfg);
    /* Handle error */
    if(FSP_SUCCESS != err)
    {
        /* GLCDC initialization failed */
        APP_ERR_PRINT("\r\n** GLCDC driver initialization FAILED **\r\n");
        APP_ERR_TRAP(err);
    }

    /* Start GLCDC display output */
    err = R_GLCDC_Start(&g_plcd_display_ctrl);
    /* Handle error */
    if(FSP_SUCCESS != err)
    {
        /* GLCDC start failed  */
        APP_ERR_PRINT("\r\n** GLCDC driver start FAILED **\r\n");
        APP_ERR_TRAP(err);
    }

    APP_PRINT("Enable Back light\n");
    R_BSP_PinAccessEnable();
//    R_IOPORT_PinWrite(&g_ioport_ctrl, PLCD_BLEN, (bsp_io_level_t)BSP_IO_LEVEL_LOW);
    R_IOPORT_PinWrite(&g_ioport_ctrl, PLCD_BLEN, (bsp_io_level_t)BSP_IO_LEVEL_HIGH);
    R_BSP_PinAccessDisable();

    APP_PRINT("Kick camera task\n");
    tk_set_flg(init_flgid, 0x01);
    tk_dly_tsk(5000);

#if 0 // Display color bar
    /* Clear LCD screen using appropriate coordinates */
    screen_display((uint16_t)X1_CO_ORDINATE, (uint16_t)Y1_CO_ORDINATE, g_hz_size, g_vr_size, BLACK);

    /* Display color bands on LCD screen */
    color_band_display();
#endif

    while(1) {
//        SEGGER_RTT_printf(0, "display task\n");
//        tk_dly_tsk(5000);
//        tk_wai_flg(camera_flgid, 0x01, TWF_ORW, &flgptn, TMO_FEVR);

        do {
            err = R_GLCDC_BufferChange(&g_plcd_display_ctrl,
                             gp_camera_buffer,
                             DISPLAY_FRAME_LAYER_1);
        }while (err == FSP_ERR_INVALID_UPDATE_TIMING);
        tk_dly_tsk(10);
    }
}
