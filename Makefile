STM32_INC=-I3rdparty/CMSIS/Include/
FREERTOS_INC=-I3rdparty/FreeRTOS/Source/include/ -I3rdparty/FreeRTOS/Source/portable/GCC/ARM_CM4F/
FREERTOS_SRCS+=3rdparty/FreeRTOS/Source/tasks.c
FREERTOS_SRCS+=3rdparty/FreeRTOS/Source/timers.c
FREERTOS_SRCS+=3rdparty/FreeRTOS/Source/queue.c
FREERTOS_SRCS+=3rdparty/FreeRTOS/Source/portable/MemMang/heap_1.c
FREERTOS_SRCS+=3rdparty/FreeRTOS/Source/list.c
FREERTOS_SRCS+=3rdparty/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c
FREERTOS_OBJS=$(subst .c,.o,$(FREERTOS_SRCS))

CXXFLAGS=-mcpu=cortex-m4 -g
ASFLAGS:=$(CXXFLAGS)
CXXFLAGS+=-Iinc $(FREERTOS_INC) $(STM32_INC) -Wall
CFLAGS:=$(CXXFLAGS)
CXXFLAGS+=-fno-rtti -fno-exceptions
LIB_SRC=$(wildcard lib/*.cpp)
LIB_OBJS=$(subst cpp,o,$(LIB_SRC))
LIB_INCS=$(wildcard inc/*.h)

SRC:=$(wildcard src/*.c) $(wildcard src/*.s)
SRC_OBJS:=$(subst .c,.o,$(SRC))
SRC_OBJS:=$(subst .s,.o,$(SRC_OBJS))

all: examples/led.bin

.SECONDARY: $(LIB_OBJS) $(FREERTOS_OBJS) $(SRC_OBJS)

lib/%.o: lib/%.cpp $(LIB_INCS)
	$(CXX) -c $< -o $@ $(CXXFLAGS)

%.bin: %.o $(LIB_OBJS) $(SRC_OBJS) $(FREERTOS_OBJS)
	-$(LD) $^ -o $@ $(LDFLAGS) -Tsrc/ram.lds

clean:
	-rm -f examples/*.bin $(FREERTOS_OBJS) $(LIB_OBJS) $(SRC_OBJS)
