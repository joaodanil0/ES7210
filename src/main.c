#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(i2c_sample, LOG_LEVEL_DBG);

// Altere conforme necessário
#define I2C_NODE DT_NODELABEL(i2c0)  // use i2c1 ou outro se for diferente
#define I2C_ADDR 0x40  // Exemplo de endereço I2C

int main(void)
{
    const struct device *i2c_dev = DEVICE_DT_GET(I2C_NODE);

    if (!device_is_ready(i2c_dev)) {
        LOG_ERR("I2C device not ready");
        return;
    }

    LOG_INF("I2C device ready");

    // Escrevendo 1 byte
    uint8_t tx_buf[2] = {0x00, 0xA5};  // exemplo: comando e valor
    int ret = i2c_write(i2c_dev, tx_buf, sizeof(tx_buf), I2C_ADDR);
    if (ret < 0) {
        LOG_ERR("I2C write failed: %d", ret);
    } else {
        LOG_INF("I2C write ok");
    }

    // Lendo 1 byte
    uint8_t rx_buf[1];
    ret = i2c_read(i2c_dev, rx_buf, sizeof(rx_buf), I2C_ADDR);
    if (ret < 0) {
        LOG_ERR("I2C read failed: %d", ret);
    } else {
        LOG_INF("I2C read ok: 0x%02X", rx_buf[0]);
    }
}
