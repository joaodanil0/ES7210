#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2s.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/logging/log.h>
#include "es7210.h"

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

#define I2S_DEV DT_LABEL(DT_NODELABEL(i2s0))
#define I2C_DEV DT_LABEL(DT_NODELABEL(i2c0))

#define BUF_SIZE 512

void main(void)
{
    const struct device *i2c_dev = device_get_binding(I2C_DEV);
    const struct device *i2s_dev = device_get_binding(I2S_DEV);

    if (!i2c_dev || !i2s_dev) {
        LOG_ERR("Devices not ready");
        return;
    }

    if (es7210_init(i2c_dev) < 0) {
        LOG_ERR("Failed to init ES7210");
        return;
    }

    struct i2s_config config = {
        .word_size = 16,
        .channels = 2,
        .format = I2S_FMT_DATA_FORMAT_I2S | I2S_FMT_BIT_CLK_INV,
        .options = I2S_OPT_FRAME_CLK_MASTER | I2S_OPT_BIT_CLK_MASTER,
        .frame_clk_freq = 16000,
        .block_size = BUF_SIZE,
        .timeout = 2000,
    };

    if (i2s_configure(i2s_dev, I2S_DIR_RX, &config) < 0) {
        LOG_ERR("I2S config failed");
        return;
    }

    i2s_trigger(i2s_dev, I2S_DIR_RX, I2S_TRIGGER_START);

    while (1) {
        void *rx;
        size_t size;

        if (i2s_read(i2s_dev, &rx, &size) == 0) {
            LOG_INF("Captured %d bytes", size);
            i2s_release(i2s_dev, I2S_DIR_RX, rx);
        } else {
            LOG_WRN("Read failed");
        }

        k_msleep(100);
    }
}