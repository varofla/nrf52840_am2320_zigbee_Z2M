#include <zephyr/kernel.h>

#include <zephyr/logging/log.h>

#include "hw.h"

#include "am2320.h"

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

int main(void) {
  hw_init();

  float temperature, humidity;
  while (true) {
    am2320_read(&temperature, &humidity);
    LOG_INF("Temperature: %.1f °C", temperature);
    LOG_INF("Humidity: %.1f %%", humidity);

    k_msleep(5000);
  }
}