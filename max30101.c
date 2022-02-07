

#include "max30101.h"
#include "string.h"

#ifdef CC1312R1

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>

#endif

#define MAX30101_ADDRESS 0x57

#define MAX30102_IR_POWER 45
#define MAX30102_RED_POWER 45
#define MAX30102_GREEN_POWER 125

#ifdef CC1312R1

static I2C_Handle i2c;
static I2C_Transaction i2cTransaction;

#endif

static void wait_100_ms()
{

#ifdef CC1312R1

    Task_sleep(100 * 1000 / Clock_tickPeriod);

#endif

}

#ifdef CC1312R1

void set_max30101_i2c_instance(I2C_Handle i2cInstance)
{

    i2c = i2cInstance;

}

#endif

static uint8_t i2c_write8(uint8_t address, uint8_t reg)
{

    uint8_t i2c_state = 0x00;
    uint8_t txBuffer = reg;

#ifdef CC1312R1

    if (i2c != NULL)
    {

        i2cTransaction.writeBuf = &txBuffer;
        i2cTransaction.readBuf = NULL;
        i2cTransaction.slaveAddress = address;

        i2cTransaction.writeCount = 1;
        i2cTransaction.readCount = 0;
        i2c_state = I2C_transfer(i2c, &i2cTransaction);

    }

#endif

    return i2c_state;

}

static uint8_t i2c_write16(uint8_t address, uint8_t reg, uint8_t value)
{

    uint8_t i2c_state = 0x00;
    uint8_t txBuffer[2] = { 0x00 };
    txBuffer[0] = reg;
    txBuffer[1] = value;

#ifdef CC1312R1

    if (i2c != NULL)
    {

        i2cTransaction.writeBuf = &txBuffer;
        i2cTransaction.readBuf = NULL;
        i2cTransaction.slaveAddress = address;

        i2cTransaction.writeCount = 2;
        i2cTransaction.readCount = 0;
        i2c_state = I2C_transfer(i2c, &i2cTransaction);

    }

#endif

    return i2c_state;

}

static uint8_t i2c_read_buffer(uint8_t address, uint8_t *read_value,
                               const uint8_t buffer_size)
{

    uint8_t i2c_state = 0x00;
    uint8_t rxBuffer[buffer_size];

#ifdef CC1312R1

    if (i2c != NULL)
    {

        i2cTransaction.writeBuf = NULL;
        i2cTransaction.readBuf = rxBuffer;
        i2cTransaction.slaveAddress = address;

        i2cTransaction.writeCount = 0;
        i2cTransaction.readCount = buffer_size;
        i2c_state = I2C_transfer(i2c, &i2cTransaction);

        memcpy(read_value, rxBuffer, buffer_size);

    }

#endif

    return i2c_state;

}

uint8_t init_max30101()
{

    uint8_t i2c_state = 0x01;

    i2c_state = i2c_state & i2c_write16(MAX30101_ADDRESS, 0x02, 0x40);

    i2c_state = i2c_state & i2c_write16(MAX30101_ADDRESS, 0x03, 0x00);

    i2c_state = i2c_state & i2c_write16(MAX30101_ADDRESS, 0x04, 0x00);

    i2c_state = i2c_state & i2c_write16(MAX30101_ADDRESS, 0x05, 0x00);

    i2c_state = i2c_state & i2c_write16(MAX30101_ADDRESS, 0x06, 0x00);

    i2c_state = i2c_state & i2c_write16(MAX30101_ADDRESS, 0x08, 0b00101111);

    i2c_state = i2c_state & i2c_write16(MAX30101_ADDRESS, 0x09, 0x07);

    i2c_state = i2c_state & i2c_write16(MAX30101_ADDRESS, 0x0A, 0b00100111);

    //RED PPG Power Setting
    i2c_state = i2c_state
            & i2c_write16(MAX30101_ADDRESS, 0x0C, MAX30102_RED_POWER);

    //IR PPG Power Setting
    i2c_state = i2c_state
            & i2c_write16(MAX30101_ADDRESS, 0x0D, MAX30102_IR_POWER);

    //GREEN PPG Power Setting
    i2c_state = i2c_state
            & i2c_write16(MAX30101_ADDRESS, 0x0E, MAX30102_GREEN_POWER);

    //GREEN PPG Power Setting
    i2c_state = i2c_state
            & i2c_write16(MAX30101_ADDRESS, 0x0F, MAX30102_GREEN_POWER);

    i2c_state = i2c_state & i2c_write16(MAX30101_ADDRESS, 0x10, 0x7F);

    i2c_state = i2c_state & i2c_write16(MAX30101_ADDRESS, 0x11, 0x21);

    i2c_state = i2c_state & i2c_write16(MAX30101_ADDRESS, 0x12, 0x03);

    i2c_state = i2c_state & i2c_write16(MAX30101_ADDRESS, 0x09, 0x83);

    for (uint8_t i = 0; i < 2; i++)
    {

        wait_100_ms();

    }

    return i2c_state;

}

uint8_t set_max30101_normal_mode()
{

    uint8_t i2c_state = i2c_write16(MAX30101_ADDRESS, 0x09, 0x07);

    return i2c_state;

}

uint8_t set_max30101_sleep_mode()
{

    uint8_t i2c_state = i2c_write16(MAX30101_ADDRESS, 0x09, 0x83);

    return i2c_state;

}

uint8_t get_max30101_ir_red_green(uint32_t *ir, uint32_t *red, uint32_t *green)
{

    uint8_t i2c_state = 0x00;
    uint8_t read_buffer[9] = { 0x00 };

    i2c_state = i2c_write8(MAX30101_ADDRESS, 0x07);

    i2c_state = i2c_state & i2c_read_buffer(MAX30101_ADDRESS, read_buffer, 9);

    uint32_t value = (read_buffer[0] << 16);
    value += (read_buffer[1] << 8);
    value += read_buffer[2];
    value = value & 0x03FFFF;
    *red = value;

    value = (read_buffer[3] << 16);
    value += (read_buffer[4] << 8);
    value += read_buffer[5];
    value = value & 0x03FFFF;
    *ir = value;

    value = (read_buffer[6] << 16);
    value += (read_buffer[7] << 8);
    value += read_buffer[8];
    value = value & 0x03FFFF;
    *green = value;

    return i2c_state;

}
