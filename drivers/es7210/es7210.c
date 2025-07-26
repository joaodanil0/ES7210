#include "es7210.h"
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/util.h>

LOG_MODULE_REGISTER(es7210, LOG_LEVEL_INF);

#define ES7210_ADDR 0x40

static int write_reg(const struct device *i2c, uint8_t reg, uint8_t val)
{
    uint8_t buf[2] = { reg, val };
    return i2c_write(i2c, buf, 2, ES7210_ADDR);
}

int es7210_init(const struct device *i2c)
{
    if (!device_is_ready(i2c)) {
        LOG_ERR("I2C device not ready");
        return -ENODEV;
    }

    // Soft reset
    write_reg(i2c, 0x00, 0xFF);
    k_msleep(50);

    // Power up analog blocks
    write_reg(i2c, 0x01, 0x3F);

    // Set I2S mode, sample rate etc (simplificado)
    write_reg(i2c, 0x06, 0x07);  // I2S 16bit
    write_reg(i2c, 0x07, 0x00);  // TDM mode off
    write_reg(i2c, 0x11, 0x60);  // Enable MICs

    LOG_INF("ES7210 initialized");
    return 0;
}
