#include <zephyr/kernel.h>

#include <zephyr/drivers/i2c.h>
#include <zephyr/logging/log.h>

#include "am2320.h"

LOG_MODULE_REGISTER(am2320_driver, LOG_LEVEL_WRN);

static const struct i2c_dt_spec s_am2320_dev = I2C_DT_SPEC_GET(DT_NODELABEL(am2320));

bool am2320_init(void) {
  if (!i2c_is_ready_dt(&s_am2320_dev)) {
    LOG_ERR("I2C bus %s is not ready!\n\r", s_am2320_dev.bus->name);
    return false;
  }

  LOG_DBG("I2C bus %s is ready!\n\r", s_am2320_dev.bus->name);
  return true;
}

static uint16_t s_crc16(uint8_t *buff, size_t len) {
  uint16_t crc = 0xffff;
  uint16_t i;
  while (len--) {
    crc ^= *buff++;
    for (i = 0; i < 8; i++) {
      if (crc & 0x01) {
        crc >>= 1;
        crc ^= 0xA001;
      } else {
        crc >>= 1;
      }
    }
  }
  return crc;
}

bool am2320_read(float *temperature_buff, float *humidity_buff) {
  if (temperature_buff == NULL || humidity_buff == NULL) {
    return false;
  }

  uint8_t send_buff[3] = {0x03, 0x00, 0x04};
  uint8_t read_buff[8];

  volatile int ret = i2c_write_dt(&s_am2320_dev, 0x00, 0);

  k_msleep(1);

  ret = i2c_write_dt(&s_am2320_dev, send_buff, 3);
  if (ret != 0) {
    LOG_ERR("write error!");
    return false;
  }

  k_msleep(2);

  ret = i2c_read_dt(&s_am2320_dev, read_buff, 8);
  if (ret != 0) {
    LOG_ERR("read error!");
    return false;
  }

  uint16_t crc = read_buff[7];
  crc <<= 8;
  crc |= read_buff[6];
  if (crc != s_crc16(read_buff, 6)) {
    LOG_ERR("CRC error!");
    return false;
  }

  uint16_t temp_temperature = (read_buff[5] | read_buff[4] << 8);
  if (temp_temperature & 0x8000) {
    temp_temperature = -(int16_t)(temp_temperature & 0x7fff);
  } else {
    temp_temperature = (int16_t)temp_temperature;
  }
  *temperature_buff = (float)temp_temperature / 10.0;
  *humidity_buff = (float)(read_buff[3] | read_buff[2] << 8) / 10.0;

  LOG_DBG("Temperature: %.1f °C, Humidity: %.1f %%", *temperature_buff, *humidity_buff);

  return true;
}