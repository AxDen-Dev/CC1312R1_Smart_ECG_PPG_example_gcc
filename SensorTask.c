#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#include <xdc/std.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/knl/Clock.h>

/* TI-RTOS Header files */
#include <ti/drivers/I2C.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/UART2.h>
#include <ti/drivers/NVS.h>
#include <ti/drivers/ADC.h>
#include <ti/drivers/SPI.h>
#include <ti/drivers/Watchdog.h>

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/cpu.h)
#include DeviceFamily_constructPath(driverlib/sys_ctrl.h)
#include DeviceFamily_constructPath(driverlib/aon_batmon.h)

#include <ti/devices/cc13x2_cc26x2/driverlib/aux_adc.h>

/* Board Header files */
#include "ti_drivers_config.h"

/* Application Header files */
#include "SensorTask.h"
#include "Protocol.h"

#include "uart_usb_in_out.h"

#include "board_define.h"
#include "si7051.h"
#include "ecg_algorithm.h"
#include "max30003.h"
#include "heartrate_spo2_algorithm.h"
#include "max30101.h"

#define SENSOR_TASK_STACK_SIZE 1024
#define SENSOR_TASK_TASK_PRIORITY   3

#define SENSOR_TASK_EVENT_ALL                         0xFFFFFFFF
#define SENSOR_TASK_SENSOR_UPDATE    (uint32_t)(1 << 0)

#define TIMER_TIMEOUT 1000
#define WATCHDOG_TIMEOUT_MS 3000

#define BUTTON_PRESS_TIMER_COUNT 5
#define BUTTON_RPESS_COUNT 3

PIN_Config pinTable[] = {

GPIO_LED_0 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,

GPIO_LED_1 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL
| PIN_DRVSTR_MAX,

GPIO_BAT_EN | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL
| PIN_DRVSTR_MAX,

GPIO_SENSOR_POWER_EN | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL
| PIN_DRVSTR_MAX,

SPI_CS_GPIO | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL
| PIN_DRVSTR_MAX,

PIN_TERMINATE

};

PIN_Config buttonTable[] = {

GPIO_BTN | PIN_INPUT_EN | PIN_PULLUP | PIN_IRQ_NEGEDGE,

                             PIN_TERMINATE

};

static Task_Params sensorTaskParams;
Task_Struct sensorTask; /* not static so you can see in ROV */
static uint8_t sensorTaskStack[SENSOR_TASK_STACK_SIZE];

Event_Struct sensorEvent; /* not static so you can see in ROV */
static Event_Handle sensorEventHandle;

Clock_Struct sensorTimerClock;
Watchdog_Handle watchdogHandle;

static PacketSendRequestCallback packetSendRequestCallback;

static PIN_State pinState;
static PIN_Handle pinHandle;

static PIN_State buttonState;
static PIN_Handle buttonHandle;

static I2C_Handle i2c;
static I2C_Params i2cParams;

static SPI_Params spiParams;
static SPI_Handle masterSpi;

static uint8_t uart_read_buf = 0x00;
static UART_Handle uart;

static uint8_t init_state = 0x00;

static int16_t temperature = 0;
static uint8_t battery_voltage = 0;
static uint8_t ir_heart_rate = 0;
static uint8_t ir_heart_rate_count = 0;
static uint8_t green_heart_rate = 0;
static uint8_t green_heart_rate_count = 0;
static uint8_t spo2 = 0;
static uint8_t spo2_count = 0;
static uint8_t ecg_heart_rate = 0;
static uint8_t ecg_heart_rate_count = 0;

static uint8_t on_off_mode = 0x01;

static uint8_t button_press_state = 0x00;
static uint8_t button_press_count = 0;
static int8_t button_press_timer_count = 0;

static uint8_t payload_size = 0;
static uint8_t payload[115];

static uint8_t sensor_task_release_state = 0x00;

extern uint8_t radio_init;
extern uint8_t mac_address[8];
extern uint32_t collection_cycle_timeout_count;
extern uint32_t collection_cycle_timer_count;

static void uartReadCallback(UART_Handle handle, void *rxBuf, size_t size);

void scCtrlReadyCallback(void)
{

}

void scTaskAlertCallback(void)
{

}

void watchdogCallback(uintptr_t watchdogHandle)
{

    while (1)
    {

    }

}

static void buttonCallback(PIN_Handle handle, PIN_Id pinId)
{

    if (init_state)
    {

        button_press_timer_count = BUTTON_PRESS_TIMER_COUNT;

        PIN_setOutputValue(pinHandle, GPIO_LED_1, 1);
        button_press_state = 0x01;

    }

}

static void sensorTimerClockCallBack(UArg arg0)
{

    if (init_state)
    {

        collection_cycle_timeout_count++;

        if (on_off_mode == 0x01)
        {

            if (collection_cycle_timeout_count >= collection_cycle_timer_count)
            {

                if (sensor_task_release_state == 0x00)
                {

                    sensor_task_release_state = 0x01;
                    Event_post(sensorEventHandle, SENSOR_TASK_SENSOR_UPDATE);

                }

                collection_cycle_timeout_count = 0;

            }

        }
        else
        {

            collection_cycle_timeout_count = 0;

        }

        if (button_press_state)
        {

            PIN_setOutputValue(pinHandle, GPIO_LED_1, 0);
            button_press_state = 0x00;

            button_press_count++;

        }

        button_press_timer_count -= 1;

        if (button_press_timer_count > 0)
        {

            if (button_press_count >= BUTTON_RPESS_COUNT)
            {

                if (on_off_mode == 0x01)
                {

                    on_off_mode = 0x00;

                }
                else
                {

                    on_off_mode = 0x01;

                }

                collection_cycle_timer_count = 0;

                PIN_setOutputValue(pinHandle, GPIO_LED_0, 1);

                button_press_timer_count = 0;
                button_press_count = 0;

            }

        }
        else
        {

            PIN_setOutputValue(pinHandle, GPIO_LED_0, 0);

            button_press_count = 0;

        }

    }

    Watchdog_clear(watchdogHandle);

}

static void wait_ms(uint32_t wait)
{

    Task_sleep(wait * 1000 / Clock_tickPeriod);

}

static void initErrorUpdate(void)
{

    for (uint8_t i = 0; i < 4; i++)
    {

        PIN_setOutputValue(pinHandle, GPIO_LED_0,
                           !PIN_getOutputValue(GPIO_LED_0));
        PIN_setOutputValue(pinHandle, GPIO_LED_1,
                           !PIN_getOutputValue(GPIO_LED_1));

        wait_ms(500);

    }

    PIN_setOutputValue(pinHandle, GPIO_LED_0, 1);
    PIN_setOutputValue(pinHandle, GPIO_LED_1, 1);

}

static uint8_t update_Heartrate_SPO2()
{

    uint8_t i2c_state = 0x01;

    uint16_t printCount = 0;

    uint32_t red = 0x00;
    uint32_t ir = 0x00;
    uint32_t green = 0x00;
    uint8_t ppgErrorCount = 0;

    i2c_state = i2c_state & set_max30101_normal_mode();

    setHeartrateMinMax(40, 180);

    if (i2c_state)
    {

        for (uint32_t i = 0; i < 3000; i++)
        {

            get_max30101_ir_red_green(&ir, &red, &green);

            if (green > MAX30101_GREEN_PROXY_THR)
            {

                updateHeartateGreen(green);

                if (updateHeartateIR(ir, red) == 0x01)
                {

                    updateSpo2RedIrAcc();

                }

                ppgErrorCount = 0;

            }
            else
            {

                ppgErrorCount++;

                if (ppgErrorCount > MAX30101_GREEN_PROXY_ERROR_COUNT)
                {

                    initHeartrate();

                    ppgErrorCount = 0;

                }

            }

            printCount++;

            if (printCount > 500)
            {

                getHeartrateValue(&ir_heart_rate, &ir_heart_rate_count,
                                  &green_heart_rate, &green_heart_rate_count);

                getSpo2Value(&spo2, &spo2_count);

                set_uart_usb_in_out_green_heartrate(green_heart_rate,
                                                    green_heart_rate_count);

                set_uart_usb_in_out_ir_heartrate(ir_heart_rate,
                                                 ir_heart_rate_count);

                set_uart_usb_in_out_spo2(spo2, spo2_count);

                printCount = 0;

            }

            wait_ms(20);

        }

    }

    initHeartrate();

    i2c_state = i2c_state & set_max30101_sleep_mode();

    return i2c_state;

}

static void update_ECG()
{

    uint16_t printCount = 0;

    init_ecg_algorithm();

    for (uint32_t i = 0; i < 7500; i++)
    {

        int32_t sample = get_max30003_ecg_voltage_sample();

        update_ecg_algorithm(sample);

        get_ecg_heartrate(&ecg_heart_rate, &ecg_heart_rate_count);

        printCount++;

        if (printCount > 1250)
        {

            set_uart_usb_in_out_ecg_heartrate(ecg_heart_rate,
                                              ecg_heart_rate_count);

            printCount = 0;

        }

        wait_ms(8);

    }

}

static void battery_value_read()
{

    ADC_Handle adc;
    ADC_Params ADCParams;
    ADC_Params_init(&ADCParams);

    adc = ADC_open(CONFIG_I2C_0, &ADCParams);

    if (adc != NULL)
    {

        uint16_t adc_value;

        PIN_setOutputValue(pinHandle, GPIO_BAT_EN, 1);

        wait_ms(20);

        int_fast16_t result = ADC_convert(adc, &adc_value);

        ADC_close(adc);

        PIN_setOutputValue(pinHandle, GPIO_BAT_EN, 0);

        if (result == ADC_STATUS_SUCCESS)
        {

            uint32_t microVolt = ADC_convertRawToMicroVolts(adc, adc_value);
            microVolt *= 3;
            microVolt /= 100000;

            battery_voltage = microVolt;

        }

    }

}

static void i2cScan()
{

    uint8_t txBuffer[2] = { 0x00 };
    uint8_t rxBuffer[2] = { 0x00 };

    I2C_Transaction i2cTransaction;

    i2cTransaction.writeBuf = txBuffer;
    i2cTransaction.readBuf = rxBuffer;
    i2cTransaction.writeCount = 1;
    i2cTransaction.readCount = 0;

    for (uint16_t i = 0; i < 128; i++)
    {

        i2cTransaction.slaveAddress = i;

        if (I2C_transfer(i2c, &i2cTransaction))
        {

            set_uart_usb_in_out_find_i2c_address(i);

            wait_ms(20);

        }

    }

}

static void initSensor(void)
{

    uint8_t i2c_state = 0x01;

    PIN_setOutputValue(pinHandle, GPIO_SENSOR_POWER_EN, 1);

    wait_ms(1000);

    I2C_Params_init(&i2cParams);
    i2cParams.bitRate = I2C_400kHz;
    i2c = I2C_open(CONFIG_I2C_0, &i2cParams);

    if (i2c == NULL)
    {

        initErrorUpdate();

        SysCtrlSystemReset();

    }

    SPI_Params_init(&spiParams);
    spiParams.mode = SPI_MASTER;
    spiParams.transferMode = SPI_MODE_BLOCKING;
    masterSpi = SPI_open(CONFIG_SPI_0, &spiParams);

    if (masterSpi == NULL)
    {

        initErrorUpdate();

        SysCtrlSystemReset();

    }

    i2cScan();

    wait_ms(100);

    set_si7051_i2c_instance(i2c);

    i2c_state = i2c_state & init_si7051();

    i2c_state = i2c_state & get_si7051_temperature(&temperature);

    wait_ms(100);

    set_max30101_i2c_instance(i2c);

    i2c_state = i2c_state & init_max30101();

    if (!i2c_state)
    {

        initErrorUpdate();

    }

    set_max30003_gpio_instance(pinHandle);

    set_max30003_spi_instance(masterSpi);

    init_max30003_mode_0();

}

static void initUartUSB()
{

    UART_Params uartParams;

    UART_Params_init(&uartParams);
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readDataMode = UART_DATA_BINARY;
    uartParams.readMode = UART_MODE_CALLBACK;
    uartParams.readCallback = uartReadCallback;
    uartParams.readReturnMode = UART_RETURN_FULL;
    uartParams.readEcho = UART_ECHO_OFF;
    uartParams.baudRate = 115200;

    uart = UART_open(CONFIG_UART_0, &uartParams);

    set_uart_usb_in_out_uart_instance(uart);

}

static void sensorTaskFunction(UArg arg0, UArg arg1)
{

    UART_init();

    ADC_init();

    I2C_init();

    SPI_init();

    pinHandle = PIN_open(&pinState, pinTable);

    if (!pinHandle)
    {

        SysCtrlSystemReset();

    }

    PIN_setOutputValue(pinHandle, GPIO_LED_0, 1);
    PIN_setOutputValue(pinHandle, GPIO_LED_1, 1);
    PIN_setOutputValue(pinHandle, GPIO_BAT_EN, 0);
    PIN_setOutputValue(pinHandle, GPIO_SENSOR_POWER_EN, 0);

    buttonHandle = PIN_open(&buttonState, buttonTable);

    if (!buttonHandle)
    {

        SysCtrlSystemReset();

    }

    PIN_registerIntCb(buttonHandle, &buttonCallback);

    initUartUSB();

    while (radio_init == 0x00)
    {

        wait_ms(100);

    }

    set_uart_usb_in_out_write_string("\n\n");
    set_uart_usb_in_out_write_string("Start AxDen PPG ECG Example");

    set_uart_usb_in_out_mac_address(mac_address);

    battery_value_read();

    initSensor();

    set_uart_usb_in_out_battery(battery_voltage);

    set_uart_usb_in_out_temperature(temperature);

    PIN_setOutputValue(pinHandle, GPIO_LED_0, 0);
    PIN_setOutputValue(pinHandle, GPIO_LED_1, 0);

    on_off_mode = 0x01;

    init_state = 0x01;

    while (1)
    {

        uint32_t events = Event_pend(sensorEventHandle, 0,
        SENSOR_TASK_EVENT_ALL,
                                     BIOS_WAIT_FOREVER);

        if (events == SENSOR_TASK_SENSOR_UPDATE)
        {

            battery_value_read();

            get_si7051_temperature(&temperature);

            update_Heartrate_SPO2();

            update_ECG();

            if (packetSendRequestCallback)
            {

                payload_size = 0;
                memset(payload, 0x00, sizeof(payload));

                payload_size = sprintf((char*) payload, "%d.%d,",
                                       (battery_voltage / 10),
                                       (battery_voltage % 10));

                payload_size += sprintf((char*) payload + payload_size,
                                        "%d.%d,", (temperature / 10),
                                        abs(temperature % 10));

                payload_size += sprintf((char*) payload + payload_size, "%d,",
                                        green_heart_rate);

                payload_size += sprintf((char*) payload + payload_size, "%d,",
                                        green_heart_rate_count);

                payload_size += sprintf((char*) payload + payload_size, "%d,",
                                        ir_heart_rate);

                payload_size += sprintf((char*) payload + payload_size, "%d,",
                                        ir_heart_rate_count);

                payload_size += sprintf((char*) payload + payload_size, "%d,",
                                        ecg_heart_rate);

                payload_size += sprintf((char*) payload + payload_size, "%d,",
                                        ecg_heart_rate_count);

                packetSendRequestCallback(payload, payload_size);

            }

        }

        sensor_task_release_state = 0x00;

    }

}

void SensorTask_init(void)
{

    Event_Params eventParam;
    Event_Params_init(&eventParam);
    Event_construct(&sensorEvent, &eventParam);
    sensorEventHandle = Event_handle(&sensorEvent);

    Task_Params_init(&sensorTaskParams);
    sensorTaskParams.stackSize = SENSOR_TASK_STACK_SIZE;
    sensorTaskParams.priority = SENSOR_TASK_TASK_PRIORITY;
    sensorTaskParams.stack = &sensorTaskStack;
    Task_construct(&sensorTask, sensorTaskFunction, &sensorTaskParams, NULL);

    Watchdog_init();

    Watchdog_Params watchdogParams;
    Watchdog_Params_init(&watchdogParams);
    watchdogParams.callbackFxn = (Watchdog_Callback) watchdogCallback;
    watchdogParams.debugStallMode = Watchdog_DEBUG_STALL_ON;
    watchdogParams.resetMode = Watchdog_RESET_ON;

    watchdogHandle = Watchdog_open(CONFIG_WATCHDOG_0, &watchdogParams);

    if (watchdogHandle == NULL)
    {

        SysCtrlSystemReset();

    }

    uint32_t reloadValue = Watchdog_convertMsToTicks(watchdogHandle,
    WATCHDOG_TIMEOUT_MS);

    if (reloadValue != 0)
    {

        Watchdog_setReload(watchdogHandle, reloadValue);

    }

    Clock_Params clockParams;
    Clock_Params_init(&clockParams);
    clockParams.period = TIMER_TIMEOUT * 1000 / Clock_tickPeriod;
    clockParams.startFlag = TRUE;
    Clock_construct(&sensorTimerClock, sensorTimerClockCallBack,
    TIMER_TIMEOUT * 1000 / Clock_tickPeriod,
                    &clockParams);

}

void SensorTask_registerPacketSendRequestCallback(
        PacketSendRequestCallback callback)
{

    packetSendRequestCallback = callback;

}

static void uartReadCallback(UART_Handle handle, void *rxBuf, size_t size)
{

    UART_read(handle, &uart_read_buf, 1);

}
