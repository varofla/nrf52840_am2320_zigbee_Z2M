#include "hw.h"

#include "driver/driver.h"

bool hw_init(void) {
  return driver_init();
}