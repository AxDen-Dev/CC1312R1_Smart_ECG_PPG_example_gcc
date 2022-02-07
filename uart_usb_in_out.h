
#ifndef UART_USB_IN_OUT_H_
#define UART_USB_IN_OUT_H_

#include <board_define.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef CC1312R1

#include <ti/drivers/I2C.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/UART2.h>
#include <ti/drivers/PIN.h>
#include "ti_drivers_config.h"

#endif

#ifdef CC1312R1

void set_uart_usb_in_out_uart_instance(UART_Handle uartInstance);

void set_uart_usb_in_out_uart2_instance(UART2_Handle uartInstance);

#endif

void set_uart_usb_in_out_write_string(char *data);

void set_uart_usb_in_out_mac_address(uint8_t *mac_address);

void set_uart_usb_in_out_find_i2c_address(uint8_t address);

void set_uart_usb_in_out_temperature(int16_t temperature);

void set_uart_usb_in_out_pressure(uint32_t pressure);

void set_uart_usb_in_out_humidity(uint8_t humidity);

void set_uart_usb_in_out_battery(uint8_t voltage);

void set_uart_usb_in_out_ir_heartrate(uint8_t heartrate, uint8_t count);

void set_uart_usb_in_out_green_heartrate(uint8_t heartrate, uint8_t count);

void set_uart_usb_in_out_spo2(uint8_t spo2, uint8_t count);

void set_uart_usb_in_out_ecg_heartrate(uint8_t heartrate, uint8_t count);

void set_uart_usb_in_out_write_value(int32_t value);

void set_uart_usb_in_out_serial_port_plotter_1(int32_t value1);

void set_uart_usb_in_out_serial_port_plotter_2(int32_t value1, int32_t value2);

void set_uart_usb_in_out_serial_port_plotter_3(int32_t value1, int32_t value2,
                                               int32_t value3);

void set_uart_usb_in_out_serial_port_plotter_4(int32_t value1, int32_t value2,
                                               int32_t value3, int32_t value4);

void set_uart_usb_in_out_serial_port_plotter_5(int32_t value1, int32_t value2,
                                               int32_t value3, int32_t value4,
                                               int32_t value5);

#endif /* UART_USB_IN_OUT_H_ */
