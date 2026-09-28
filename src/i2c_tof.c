#include "common_utils.h"
//#include "kernel_app_interface.h"
#include "hal_data.h"
#include "i2c_tof.h"

#define VL53L0X_REG_SYSRANGE_START        0x00
#define VL53L0X_REG_RESULT_RANGE_STATUS   0x14
#define VL53L0X_REG_RESULT_DISTANCE       0x1E

volatile bool g_i2c_tx_complete = false;
volatile bool g_i2c_rx_complete = false;

void iic_tof_init(void)
{
    fsp_err_t err;

    /* Init i2c for TOF sensor */
    err = R_IIC_MASTER_Open(&g_i2c_tof_ctrl, &g_i2c_tof_cfg);
    if (err != FSP_SUCCESS)
    {
        SEGGER_RTT_printf(0, "I2C Open error\n");
        return;
    }
    SEGGER_RTT_printf(0, "Open IIC for VL53L0X\n");
}

void iic_tof_callback(i2c_master_callback_args_t *p_args)
{
    switch (p_args->event)
    {
        case I2C_MASTER_EVENT_TX_COMPLETE:
            g_i2c_tx_complete = true;
            break;

        case I2C_MASTER_EVENT_RX_COMPLETE:
            g_i2c_rx_complete = true;
            break;
        default:
            break;
    }
}

static bool iic_tof_write_reg(uint8_t reg, uint8_t value)
{
    fsp_err_t err;

    g_i2c_tx_complete = false;
    uint8_t buf[2] = { reg, value };

    err = R_IIC_MASTER_Write(&g_i2c_tof_ctrl, buf, 2, false);
    if (err != FSP_SUCCESS) return false;

    int timeout = 10;
    while (!g_i2c_tx_complete && timeout--) {
        R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);
    }
    return g_i2c_tx_complete;
}

static bool iic_tof_read_reg(uint8_t reg, uint8_t *value)
{
    fsp_err_t err;
    // レジスタ指定（Repeated Start）
    g_i2c_tx_complete = false;
    err = R_IIC_MASTER_Write(&g_i2c_tof_ctrl, &reg, 1, true);
    if (err != FSP_SUCCESS) return false;

    int timeout = 10;
    while (!g_i2c_tx_complete && timeout--) {
        R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);
    }
    if (!g_i2c_tx_complete) return false;

    // 読み出し
    g_i2c_rx_complete = false;
    err = R_IIC_MASTER_Read(&g_i2c_tof_ctrl, value, 1, false);
    if (err != FSP_SUCCESS) return false;

    timeout = 10;
    while (!g_i2c_rx_complete && timeout--) {
        R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);
    }
    return g_i2c_rx_complete;
}

uint16_t tof_read_distance(void)
{
    uint8_t status = 0;
    uint8_t high = 0, low = 0;

    // 測距開始
    iic_tof_write_reg(VL53L0X_REG_SYSRANGE_START, 0x01);

    // 測距完了待ち（最大 20ms）
    for (int i = 0; i < 20; i++)
    {
        iic_tof_read_reg(VL53L0X_REG_RESULT_RANGE_STATUS, &status);
        if (status & 0x01) break;  // ready
        R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);
    }

    // 距離データ読み出し（16bit）
    iic_tof_read_reg(VL53L0X_REG_RESULT_DISTANCE, &high);
    iic_tof_read_reg(VL53L0X_REG_RESULT_DISTANCE + 1, &low);

    return (high << 8) | low;
}
