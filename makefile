CROSS_COMPILE=arm-none-eabi-
QEMU_STM32 ?= ../qemu_stm32/arm-softmmu/qemu-system-arm

ARCHCM=CM3
VENDOR=ST
PLAT=STM32F10x
CMSIS_LIB=libraries/CMSIS/$(ARCHCM)
STM32_LIB=libraries/STM32F10x_StdPeriph_Driver

CMSIS_PLAT_SRC = $(CMSIS_LIB)/DeviceSupport/$(VENDOR)/$(PLAT)

FREERTOS_SRC = libraries/FreeRTOS
FREERTOS_INC = $(FREERTOS_SRC)/include/
FREERTOS_PORT_INC = $(FREERTOS_SRC)/portable/GCC/ARM_$(ARCHCM)/

PSEFM_LIB = plibs
PSEFM_APP = apps

all: main.bin

main.bin: main.c
	$(CROSS_COMPILE)gcc \
		-Wl,-Tmain.ld -nostartfiles \
		-I. -I$(FREERTOS_INC) -I$(FREERTOS_PORT_INC) \
		-Ilibraries/CMSIS/CM3/CoreSupport \
		-Ilibraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x \
		-Ilibraries/STM32F10x_StdPeriph_Driver/inc \
		-Iapps \
		-Iplibs \
		-Iplibs/include \
		-fno-common \
		-gdwarf-2 -g3 \
		-mcpu=cortex-m3 -mthumb \
		-o main.elf \
		\
		$(CMSIS_LIB)/CoreSupport/core_cm3.c \
		$(CMSIS_PLAT_SRC)/system_stm32f10x.c \
		$(CMSIS_PLAT_SRC)/startup/gcc_ride7/startup_stm32f10x_md.s \
		$(STM32_LIB)/src/stm32f10x_rcc.c \
		$(STM32_LIB)/src/stm32f10x_gpio.c \
		$(STM32_LIB)/src/stm32f10x_usart.c \
		$(STM32_LIB)/src/stm32f10x_exti.c \
		$(STM32_LIB)/src/misc.c \
		\
		$(FREERTOS_SRC)/croutine.c \
		$(FREERTOS_SRC)/list.c \
		$(FREERTOS_SRC)/queue.c \
		$(FREERTOS_SRC)/tasks.c \
		$(FREERTOS_SRC)/portable/GCC/ARM_CM3/port.c \
		$(FREERTOS_SRC)/portable/MemMang/heap_2.c \
		\
		$(PSEFM_LIB)/mode.c \
		$(PSEFM_LIB)/ptask.c \
		$(PSEFM_LIB)/servant.c \
		$(PSEFM_LIB)/event.c \
		$(PSEFM_LIB)/ef.c \
		$(PSEFM_LIB)/list_internal.c \
		$(PSEFM_LIB)/modeltime.c \
		$(PSEFM_LIB)/PSPMconfigure.c \
		$(PSEFM_LIB)/hashtable.c \
		\
		$(PSEFM_APP)/app.c \
		$(PSEFM_APP)/stm32_p103.c \
		\
		main.c
		$(CROSS_COMPILE)objcopy -Obinary main.elf main.bin
		$(CROSS_COMPILE)objdump -S main.elf > main.list

qemu: main.bin $(QEMU_STM32)
	#$(QEMU_STM32) -monitor stdio -M stm32-p103 -kernel main.bin
	$(QEMU_STM32) -monitor stdio -M stm32-p103 -nographic -kernel main.bin -serial pty

qemugdb: main.bin $(QEMU_STM32)
	$(QEMU_STM32) -M stm32-p103 -gdb tcp::3333 -S -kernel main.bin

qemuauto: main.bin
	bash emulate.sh main.bin

emu: main.bin


clean:
	rm -f *.elf *.bin *.list
