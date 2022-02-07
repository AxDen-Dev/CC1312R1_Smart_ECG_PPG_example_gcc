
#ifndef MAX30101_H_
#define MAX30101_H_

#include <stdint.h>
#include "board_define.h"

#ifdef CC1312R1

#include <ti/drivers/I2C.h>
#include "ti_drivers_config.h"

#endif

#ifdef CC1312R1

void set_max30101_i2c_instance(I2C_Handle i2cInstance);

#endif

uint8_t init_max30101();

uint8_t set_max30101_normal_mode();

uint8_t set_max30101_sleep_mode();

uint8_t get_max30101_ir_red_green(uint32_t *ir, uint32_t *red, uint32_t *green);

#endif /* MAX30101_H_ */
