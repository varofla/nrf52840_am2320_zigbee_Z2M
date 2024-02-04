#include "driver.h"

#include "am2320/am2320.h"

bool driver_init(void) {
  bool ret = true;
  ret &= am2320_init();

  return ret;
}