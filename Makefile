#====================================================================#
#Output files
EXECUTABLE=stm32_executable.elf
BIN_IMAGE=stm32_bin_image.bin

#======================================================================#
#Cross Compiler
CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy

#======================================================================#
#Flags
CFLAGS= -g -mlittle-endian -mthumb -std=c99
CFLAGS+=-mcpu=cortex-m3
CFLAGS+=-D USE_STDPERIPH_DRIVER
CFLAGS+=-I./
CFLAGS+=-D STM32F10X_HD
#use newlib nano
CFLAGS+=--specs=nano.specs -u _printf_float  
#no semi-host
CFLAGS+=--specs=nosys.specs
#soft floating point
CFLAGS+=-mfloat-abi=soft
#stm32-flash
CFLAGS+=-Wl,-T,stm32_flash.ld
#LDFLAGS
LDFLAGS+=-lm -lc -lgcc


#======================================================================#
CFLAGS+=-I./
#Libraries
CFLAGS+=-I./MPU6050_lib
#Stm32 libraries
ST_LIB=./lib/STM32F10x_StdPeriph_Driver
ST_LIB_add=./STM32_MPU9150eMPL/Libraries

#CMSIS libraries
CFLAGS+=-I./lib/CMSIS/CM3/CoreSupport

#StdPeriph includes
CFLAGS+=-I$(ST_LIB)/inc/
CFLAGS+=-I./lib/CMSIS/CM3/DeviceSupport/ST/STM32F10x/
CFLAGS+=-I$(ST_LIB_add)/STM32_CPAL_Driver/inc/
CFLAGS+=-I$(ST_LIB_add)/STM32_CPAL_Driver/devices/stm32f10x/

#programs added
CFLAGS+=-I./USER/inc/
CFLAGS+=-I./STM32_MPU9150eMPL/Application/inc/
#CFLAGS+=-I./STM32_MPU9150eMPL/Libraries/CMSIS/Include/

#CFLAGS+=-I./adding/


#======================================================================#
#setup system clock
SRC=./system_stm32f10x.c
#StdPeriph
SRC+=$(ST_LIB)/src/misc.c \
	$(ST_LIB)/src/stm32f10x_rcc.c \
	$(ST_LIB)/src/stm32f10x_gpio.c \
	$(ST_LIB)/src/stm32f10x_i2c.c \
	$(ST_LIB)/src/stm32f10x_adc.c \
	$(ST_LIB)/src/stm32f10x_bkp.c \
	$(ST_LIB)/src/stm32f10x_can.c \
	$(ST_LIB)/src/stm32f10x_crc.c \
	$(ST_LIB)/src/stm32f10x_dac.c \
	$(ST_LIB)/src/stm32f10x_dbgmcu.c \
	$(ST_LIB)/src/stm32f10x_dma.c \
	$(ST_LIB)/src/stm32f10x_exti.c \
	$(ST_LIB)/src/stm32f10x_flash.c \
	$(ST_LIB)/src/stm32f10x_fsmc.c \
	$(ST_LIB)/src/stm32f10x_iwdg.c \
	$(ST_LIB)/src/stm32f10x_pwr.c \
	$(ST_LIB)/src/stm32f10x_rtc.c \
	$(ST_LIB)/src/stm32f10x_sdio.c \
	$(ST_LIB)/src/stm32f10x_spi.c \
	$(ST_LIB)/src/stm32f10x_tim.c \
	$(ST_LIB)/src/stm32f10x_usart.c \
	$(ST_LIB)/src/stm32f10x_wwdg.c \
	$(ST_LIB_add)/STM32_CPAL_Driver/src/cpal_i2c.c \
	$(ST_LIB_add)/STM32_CPAL_Driver/src/cpal_hal.c \
	$(ST_LIB_add)/STM32_CPAL_Driver/src/cpal_usercallback_template.c \
	$(ST_LIB_add)/STM32_CPAL_Driver/devices/stm32f10x/cpal_i2c_hal_stm32f10x.c
    

#MPU6050 Lib
SRC+=./MPU6050_lib/MPU6050.c
#Major programs
SRC+=./main.c 
#programs added
SRC+=./USER/src/adc.c \
     ./USER/src/clock.c \
     ./USER/src/dma.c \
     ./USER/src/exti.c \
     ./USER/src/functions.c \
     ./USER/src/global.c \
     ./USER/src/gpio.c \
     ./USER/src/initial.c \
     ./USER/src/nvic.c \
     ./USER/src/STM32_DELAY.c \
     ./USER/src/timer.c \
     ./USER/src/USART.c \
     ./USER/src/printf.c \
     ./USER/src/gps_functions.c \
     ./STM32_MPU9150eMPL/Application/src/stm32_CPAL_mpu9150.c \
     ./USER/src/sensor.c 


   



#======================================================================#
#STM32 startup file
STARTUP=./lib/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/gcc_ride7/startup_stm32f10x_hd.s


#======================================================================#
#Make rules

#Make all
all:$(BIN_IMAGE)

$(BIN_IMAGE):$(EXECUTABLE)
	$(OBJCOPY) -O binary $^ $@

STARTUP_OBJ = startup_stm32f10x_hd.o

$(STARTUP_OBJ): $(STARTUP) 
	$(CC) $(CFLAGS) $^ -c $(STARTUP)

$(EXECUTABLE):$(SRC) $(STARTUP_OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

#Make clean
clean:
	rm -rf $(EXECUTABLE)
	rm -rf $(BIN_IMAGE)
#Make flash
flash:
	st-flash write $(BIN_IMAGE) 0x8000000
openocd:
	openocd -f "./commom/openocd.cfg"
gdb:
	arm-none-eabi-gdb -x ./commom/gdb_init.gdb
gdbtui:
	arm-none-eabi-gdb -tui -x ./commom/gdb_init.gdb
#======================================================================
.PHONY:all clean flash
