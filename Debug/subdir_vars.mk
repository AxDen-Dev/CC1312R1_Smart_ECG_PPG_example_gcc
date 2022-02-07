################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
SYSCFG_SRCS += \
../rfWsnNode.syscfg 

LDS_SRCS += \
../cc13x2_cc26x2_tirtos.lds 

C_SRCS += \
../RadioTask.c \
../SensorTask.c \
../ecg_algorithm.c \
../heartrate_spo2_algorithm.c \
../main.c \
../max30003.c \
../max30101.c \
./syscfg/ti_easylink_config.c \
./syscfg/ti_devices_config.c \
./syscfg/ti_radio_config.c \
./syscfg/ti_drivers_config.c \
../si7051.c \
../uart_usb_in_out.c 

GEN_FILES += \
./syscfg/ti_easylink_config.c \
./syscfg/device_config.opt \
./syscfg/ti_devices_config.c \
./syscfg/ti_radio_config.c \
./syscfg/ti_drivers_config.c 

GEN_MISC_DIRS += \
./syscfg/ 

C_DEPS += \
./RadioTask.d \
./SensorTask.d \
./ecg_algorithm.d \
./heartrate_spo2_algorithm.d \
./main.d \
./max30003.d \
./max30101.d \
./syscfg/ti_easylink_config.d \
./syscfg/ti_devices_config.d \
./syscfg/ti_radio_config.d \
./syscfg/ti_drivers_config.d \
./si7051.d \
./uart_usb_in_out.d 

GEN_OPTS += \
./syscfg/device_config.opt 

OBJS += \
./RadioTask.o \
./SensorTask.o \
./ecg_algorithm.o \
./heartrate_spo2_algorithm.o \
./main.o \
./max30003.o \
./max30101.o \
./syscfg/ti_easylink_config.o \
./syscfg/ti_devices_config.o \
./syscfg/ti_radio_config.o \
./syscfg/ti_drivers_config.o \
./si7051.o \
./uart_usb_in_out.o 

GEN_MISC_FILES += \
./syscfg/ti_easylink_config.h \
./syscfg/ti_radio_config.h \
./syscfg/ti_drivers_config.h \
./syscfg/ti_utils_build_linker.cmd.genlibs \
./syscfg/syscfg_c.rov.xs \
./syscfg/ti_utils_runtime_model.gv \
./syscfg/ti_utils_runtime_Makefile 

GEN_MISC_DIRS__QUOTED += \
"syscfg/" 

OBJS__QUOTED += \
"RadioTask.o" \
"SensorTask.o" \
"ecg_algorithm.o" \
"heartrate_spo2_algorithm.o" \
"main.o" \
"max30003.o" \
"max30101.o" \
"syscfg/ti_easylink_config.o" \
"syscfg/ti_devices_config.o" \
"syscfg/ti_radio_config.o" \
"syscfg/ti_drivers_config.o" \
"si7051.o" \
"uart_usb_in_out.o" 

GEN_MISC_FILES__QUOTED += \
"syscfg/ti_easylink_config.h" \
"syscfg/ti_radio_config.h" \
"syscfg/ti_drivers_config.h" \
"syscfg/ti_utils_build_linker.cmd.genlibs" \
"syscfg/syscfg_c.rov.xs" \
"syscfg/ti_utils_runtime_model.gv" \
"syscfg/ti_utils_runtime_Makefile" 

C_DEPS__QUOTED += \
"RadioTask.d" \
"SensorTask.d" \
"ecg_algorithm.d" \
"heartrate_spo2_algorithm.d" \
"main.d" \
"max30003.d" \
"max30101.d" \
"syscfg/ti_easylink_config.d" \
"syscfg/ti_devices_config.d" \
"syscfg/ti_radio_config.d" \
"syscfg/ti_drivers_config.d" \
"si7051.d" \
"uart_usb_in_out.d" 

GEN_FILES__QUOTED += \
"syscfg/ti_easylink_config.c" \
"syscfg/device_config.opt" \
"syscfg/ti_devices_config.c" \
"syscfg/ti_radio_config.c" \
"syscfg/ti_drivers_config.c" 

C_SRCS__QUOTED += \
"../RadioTask.c" \
"../SensorTask.c" \
"../ecg_algorithm.c" \
"../heartrate_spo2_algorithm.c" \
"../main.c" \
"../max30003.c" \
"../max30101.c" \
"./syscfg/ti_easylink_config.c" \
"./syscfg/ti_devices_config.c" \
"./syscfg/ti_radio_config.c" \
"./syscfg/ti_drivers_config.c" \
"../si7051.c" \
"../uart_usb_in_out.c" 

SYSCFG_SRCS__QUOTED += \
"../rfWsnNode.syscfg" 


