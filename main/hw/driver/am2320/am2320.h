#ifndef MAIN_HW_DRIVER_AM2320_AM2320
#define MAIN_HW_DRIVER_AM2320_AM2320

#include <stdbool.h>

bool am2320_init(void);
bool am2320_read(float *temperature_buff, float *humidity_buff);

#endif /* MAIN_HW_DRIVER_AM2320_AM2320 */
