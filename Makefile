include usb.mk

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

ARCH=$(shell arch)
ifeq ($(ARCH),armv7l)
	PREFIX=
endif
PREFIX?=arm-none-eabi-
CXXFLAGS=-mcpu=cortex-m4 -g -mthumb -mfpu=vfpv4-d16 -mfloat-abi=hard
ASFLAGS:=$(CXXFLAGS)
CXXFLAGS+=-Iinc $(FREERTOS_INC) $(STM32_INC) $(USB_INC) -Wall -fno-stack-protector
CFLAGS:=$(CXXFLAGS)
CXXFLAGS+=-fno-rtti -fno-exceptions -std=c++11
LIB_SRC=$(wildcard lib/*.cpp)
LIB_OBJS=$(subst cpp,o,$(LIB_SRC))
LIB_INCS=$(wildcard inc/*.h)

CC=$(PREFIX)gcc
CXX=$(PREFIX)g++
LD=$(PREFIX)ld
AS=$(PREFIX)as

SRC:=$(wildcard src/*.c) $(wildcard src/*.s)
SRC_OBJS:=$(subst .c,.o,$(SRC))
SRC_OBJS:=$(subst .s,.o,$(SRC_OBJS))

all: examples/led.ram examples/led.flash

.SECONDARY: $(LIB_OBJS) $(FREERTOS_OBJS) $(SRC_OBJS) $(USB_OBJS)

lib/%.o: lib/%.cpp $(LIB_INCS)
	$(CXX) -c $< -o $@ $(CXXFLAGS)

%.ram: %.o $(LIB_OBJS) $(SRC_OBJS) $(FREERTOS_OBJS) $(USB_OBJS)
	$(LD) $^ -o $@ $(LDFLAGS) -Tsrc/ram.lds

%.flash: %.o $(LIB_OBJS) $(SRC_OBJS) $(FREERTOS_OBJS) $(USB_OBJS)
	$(LD) $^ -o $@ $(LDFLAGS) -Tsrc/flash.lds

clean:
	-rm -f examples/*.flash examples/*.ram} $(FREERTOS_OBJS) $(LIB_OBJS) $(SRC_OBJS) $(USB_OBJS)
