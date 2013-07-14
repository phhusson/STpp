FREERTOS_INC=-IFreeRTOS/Source/include/ -IFreeRTOS/Source/portable/GCC/ARM_CM4F/
FREERTOS_SRCS+=FreeRTOS/Source/tasks.c
FREERTOS_SRCS+=FreeRTOS/Source/timers.c
FREERTOS_SRCS+=FreeRTOS/Source/queue.c
FREERTOS_SRCS+=FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c
FREERTOS_SRCS+=FreeRTOS/Source/portable/MemMang/heap_4.c
FREERTOS_SRCS+=FreeRTOS/Source/list.c
FREERTOS_OBJS=$(subst .c,.o,$(FREERTOS_SRCS))

CXXFLAGS=-Iinc $(FREERTOS_INC) -mcpu=cortex-m4
CFLAGS:=$(CXXFLAGS)
CXXFLAGS+=-fno-rtti -fno-exceptions
LIB_SRC=$(wildcard lib/*.cpp)
LIB_OBJS=$(subst cpp,o,$(LIB_SRC))
LIB_INCS=$(wildcard inc/*.h)

all: examples/led.bin

.SECONDARY: $(LIB_OBJS) $(FREERTOS_OBJS)

lib/%.o: lib/%.cpp $(LIB_INCS)
	g++ -c $< -o $@ $(CXXFLAGS)

%.bin: %.o $(LIB_OBJS) $(FREERTOS_OBJS)
	-ld $^ -o $@ $(LDFLAGS) -emain
	cp $< $@

clean:
	-rm -f examples/*.bin $(FREERTOS_OBJS) $(LIB_OBJS)
