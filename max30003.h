
#ifndef MAX30003_H_
#define MAX30003_H_

#include <stdint.h>
#include <stdbool.h>
#include "board_define.h"

#ifdef CC1312R1

#include <ti/drivers/SPI.h>
#include <ti/drivers/PIN.h>
#include "ti_drivers_config.h"

#endif

#ifdef CC1312R1

void set_max30003_gpio_instance(PIN_Handle pinHandleInstance);

void set_max30003_spi_instance(SPI_Handle spiHandleInstance);

#endif

uint8_t get_max30003_rev_id();

void init_max30003_mode_0();

void init_max30003_mode_1();

void set_max30003_sync();

void set_max30003_fifo_reset();

int32_t get_max30003_ecg_voltage_sample();

uint32_t get_max30003_rtor_interval();

#endif /* MAX30003_H_ */
