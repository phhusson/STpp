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
CXXFLAGS+=-Iinc -Iplat-inc $(FREERTOS_INC) $(STM32_INC) $(USB_INC) -Wall -fno-stack-protector -O3 -DARM_MATH_CM4 -D__FPU_PRESENT=1
CFLAGS:=$(CXXFLAGS)
CXXFLAGS+=-fno-rtti -fno-exceptions -std=c++11
LIB_SRC=$(wildcard lib/*.cpp)
LIB_OBJS=$(subst cpp,o,$(LIB_SRC)) lib/debug.o
LIB_INCS=$(wildcard inc/*.h)

PLAT?=stm
PLAT_SRC=$(wildcard plat-$(PLAT)/*.cpp)
PLAT_OBJS=$(subst cpp,o,$(PLAT_SRC))
PLAT_INCS=$(wildcard plat-inc/*.h)

CC=$(PREFIX)gcc
CXX=$(PREFIX)g++
LD=$(PREFIX)ld
AS=$(PREFIX)as

SRC:=$(wildcard src/*.c) $(wildcard src/*.s)
SRC_OBJS:=$(subst .c,.o,$(SRC))
SRC_OBJS:=$(subst .s,.o,$(SRC_OBJS))

TARGETS:=led capa ax12 button timer shell
EXECS:=$(addprefix examples/,$(TARGETS))
EXECS:=$(addsuffix .flash,$(EXECS)) $(addsuffix .ram,$(EXECS))

all: $(EXECS)

lib/static.o: lib/static.cpp lib/Board.static.h

.SECONDARY: $(LIB_OBJS) $(FREERTOS_OBJS) $(SRC_OBJS) $(USB_OBJS) $(PLAT_OBJS)

#lib/%.o: lib/%.cpp $(LIB_INCS) $(PLAT_INCS)
#	$(CXX) -c $< -o $@ $(CXXFLAGS)

%.o: lib/%.cpp $(LIB_INCS) $(PLAT_INCS)
	$(CXX) -c $< -o $@ $(CXXFLAGS)

%.ram: %.o $(LIB_OBJS) $(PLAT_OBJS) $(SRC_OBJS) $(FREERTOS_OBJS) $(USB_OBJS)
	$(LD) $^ -o $@ $(LDFLAGS) -Tsrc/ram.lds

%.flash: %.o $(LIB_OBJS) $(PLAT_OBJS) $(SRC_OBJS) $(FREERTOS_OBJS) $(USB_OBJS)
	$(LD) $^ -o $@ $(LDFLAGS) -Tsrc/flash.lds

clean:
	-rm -f examples/*.flash examples/*.ram $(FREERTOS_OBJS) $(LIB_OBJS) $(SRC_OBJS) $(USB_OBJS) $(PLAT_OBJS)
