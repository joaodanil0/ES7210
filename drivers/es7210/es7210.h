#ifndef ES7210_H
#define ES7210_H

#include <zephyr/device.h>

int es7210_init(const struct device *i2c_dev);

#endif