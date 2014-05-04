
STM32_INC=-I3rdparty/CMSIS/Include/
FREERTOS_BASE=3rdparty/FreeRTOS/Source
FREERTOS_INC=-I$(FREERTOS_BASE)/include/ -I$(FREERTOS_BASE)/portable/GCC/ARM_CM4F/
FREERTOS_SRCS+=$(FREERTOS_BASE)/tasks.c
FREERTOS_SRCS+=$(FREERTOS_BASE)/timers.c
FREERTOS_SRCS+=$(FREERTOS_BASE)/queue.c
FREERTOS_SRCS+=$(FREERTOS_BASE)/portable/MemMang/heap_1.c
FREERTOS_SRCS+=$(FREERTOS_BASE)/list.c
FREERTOS_SRCS+=$(FREERTOS_BASE)/portable/GCC/ARM_CM4F/port.c
FREERTOS_OBJS=$(subst .c,.o,$(FREERTOS_SRCS))

HOST_ARCH=$(shell arch)
PLAT?=stm
ifeq ($(PLAT),stm)
include usb.mk
ARCH_CFLAGS=-DARCH=stm
ifeq ($(HOST_ARCH),armv7l)
	PREFIX=
endif
PREFIX?=arm-none-eabi-
SRC:=$(wildcard src/*.c) $(wildcard src/*.s)
SRC_OBJS:=$(subst .c,.o,$(SRC))
SRC_OBJS:=$(subst .s,.o,$(SRC_OBJS))
CXXFLAGS=-mcpu=cortex-m4 -g -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard
LDFLAGS=-flto 
else
ARCH_CFLAGS=-DARCH=unix
CXXFLAGS=-g
FREERTOS_OBJS=
LDFLAGS=-lpthread
endif

ASFLAGS:=$(CXXFLAGS)
#LDLIBS:=$(shell $(PREFIX)gcc -print-libgcc-file-name)
#CXXFLAGS+=-Iinc -Iplat-inc $(FREERTOS_INC) $(STM32_INC) $(USB_INC) -Wall -fno-stack-protector -O3 -DARM_MATH_CM4 -D__FPU_PRESENT=1 $(ARCH_CFLAGS) -fshort-double -fdata-sections -ffunction-sections -Wextra -Werror
CXXFLAGS+=-Iinc -Idrivers/inc -Iplat-inc $(FREERTOS_INC) $(STM32_INC) $(USB_INC) -Wall -fno-stack-protector -O3 -DARM_MATH_CM4 -D__FPU_PRESENT=1 $(ARCH_CFLAGS) -fshort-double -Wextra -Werror
CFLAGS:=$(CXXFLAGS)
CXXFLAGS+=-fno-rtti -fno-exceptions -std=c++11

LIB_SRC=$(wildcard lib/*.cpp) $(wildcard lib/*.c)
LIB_OBJS=$(subst cpp,o,$(LIB_SRC)) lib/debug.o
LIB_OBJS:=$(subst .c,.o,$(LIB_OBJS))
LIB_INCS=$(wildcard inc/*.h)

DRIVERS_SRC=$(wildcard drivers/*.cpp) $(wildcard drivers/*.c)
DRIVERS_OBJS=$(subst .cpp,.o,$(DRIVERS_SRC))
DRIVERS_OBJS:=$(subst .c,.o,$(DRIVERS_OBJS))
DRIVERS_INCS=$(wildcard drivers/inc/*.h)

PLAT_SRC=$(wildcard plat-$(PLAT)/*.cpp)
PLAT_OBJS=$(subst cpp,o,$(PLAT_SRC))
PLAT_INCS=$(wildcard plat-inc/*.h)

CC=$(PREFIX)gcc
CXX=$(PREFIX)g++
LD=$(PREFIX)ld
AS=$(PREFIX)as

TARGETS:=led capa ax12 button timer shell
EXECS:=$(addprefix examples/,$(TARGETS))
ifeq ($(PLAT),stm)
EXECS:=$(addsuffix .flash,$(EXECS)) $(addsuffix .ram,$(EXECS))
endif

BAD_FILES:=3rdparty/STM32_USB-Host-Device_Lib_V2.1.0/Libraries/STM32_USB_OTG_Driver//src/usb_core.o lib/arm_sin_cos_f32.o
BAD_FILES+=3rdparty/STM32_USB-Host-Device_Lib_V2.1.0/Libraries/STM32_USB_OTG_Driver//src/usb_dcd_int.o
BAD_FILES+=3rdparty/STM32_USB-Host-Device_Lib_V2.1.0/Libraries/STM32_USB_Device_Library//Core/src/usbd_core.o
BAD_FILES+=3rdparty/STM32_USB-Host-Device_Lib_V2.1.0/Libraries/STM32_USB_Device_Library//Core/src/usbd_req.o
BAD_FILES+=3rdparty/STM32_USB-Host-Device_Lib_V2.1.0/Libraries/STM32_USB_Device_Library//Class/hid/src/usbd_hid_core.o
BAD_FILES+=3rdparty/STM32_USB-Host-Device_Lib_V2.1.0/Libraries/STM32_USB_Device_Library//Class/cdc/src/usbd_cdc_core.o
BAD_FILES+=3rdparty/STM32_USB-Host-Device_Lib_V2.1.0/Libraries/STM32_USB_HOST_Library//Core/src/usbh_core.o
BAD_FILES+=3rdparty/STM32_USB-Host-Device_Lib_V2.1.0/Libraries/STM32_USB_HOST_Library//Core/src/usbh_hcs.o
BAD_FILES+=3rdparty/STM32_USB-Host-Device_Lib_V2.1.0/Libraries/STM32_USB_HOST_Library//Core/src/usbh_ioreq.o
$(BAD_FILES): CFLAGS+=-Wno-sign-compare -Wno-strict-aliasing -Wno-unused-parameter

all: $(EXECS)

lib/static.o: lib/static.cpp lib/Board.static.h

.SECONDARY: $(LIB_OBJS) $(FREERTOS_OBJS) $(SRC_OBJS) $(USB_OBJS) $(PLAT_OBJS) $(DRIVERS_OBJS) $(EXECS)

lib/%.o: lib/%.cpp $(LIB_INCS) $(PLAT_INCS) $(DRIVER_INCS)
	$(CXX) -c $< -o $@ $(CXXFLAGS)

drivers/%.o: drivers/%.cpp $(DRIVERS_INCS) $(PLAT_INCS)
	$(CXX) -c $< -o $@ $(CXXFLAGS)

ifeq ($(PLAT),stm)
%.ram: %.o $(LIB_OBJS) $(PLAT_OBJS) $(SRC_OBJS) $(FREERTOS_OBJS) $(USB_OBJS) $(DRIVERS_OBJS)
	$(LD) $^ -o $@ $(LDFLAGS) -Tsrc/ram.lds $(LDLIBS)

%.flash: %.o $(LIB_OBJS) $(PLAT_OBJS) $(SRC_OBJS) $(FREERTOS_OBJS) $(USB_OBJS) $(DRIVERS_OBJS)
	$(LD) $^ -o $@ $(LDFLAGS) -Tsrc/flash.lds $(LDLIBS)
else
examples/%: examples/%.o $(LIB_OBJS) $(PLAT_OBJS) $(SRC_OBJS) $(FREERTOS_OBJS) $(USB_OBJS)
	g++ $^ -o $@ $(LDFLAGS)
endif

clean:
	-rm -f examples/*.flash examples/*.ram $(FREERTOS_OBJS) $(LIB_OBJS) $(SRC_OBJS) $(USB_OBJS) $(PLAT_OBJS) $(DRIVERS_OBJS)
