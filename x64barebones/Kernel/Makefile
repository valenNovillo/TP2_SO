include Makefile.inc

KERNEL=kernel.bin
SOURCES=$(wildcard *.c)
SOURCES_DRIVERS=$(wildcard Drivers/*.c)
SOURCES_INTERRUPTIONS=$(wildcard Interruptions/*.c)
SOURCES_PROCESSES=$(wildcard Processes/*.c)
SOURCES_SEMAPHORES=$(wildcard Semaphore/*.c)
SOURCES_ASM=$(wildcard asm/*.asm)
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_DRIVERS=$(SOURCES_DRIVERS:.c=.o)
OBJECTS_INTERRUPTIONS=$(SOURCES_INTERRUPTIONS:.c=.o)
OBJECTS_PROCESSES=$(SOURCES_PROCESSES:.c=.o)
OBJECTS_SEMAPHORES=$(SOURCES_SEMAPHORES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
LOADERSRC=loader.asm
LOADEROBJECT=$(LOADERSRC:.asm=.o)

ifdef BUDDY_MODE
    SOURCES_MMANAGER=$(wildcard MemoryManager/buddyManager.c)
    OBJECTS_MMANAGER=$(SOURCES_MMANAGER:.c=.o)
    GCCFLAGS += -DBUDDY_MODE
else
    SOURCES_MMANAGER=$(wildcard MemoryManager/memoryManager.c)
    OBJECTS_MMANAGER=$(SOURCES_MMANAGER:.c=.o)
endif

all: $(KERNEL)

$(KERNEL): $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(OBJECTS_DRIVERS) $(OBJECTS_INTERRUPTIONS) $(OBJECTS_MMANAGER) $(OBJECTS_PROCESSES) $(OBJECTS_SEMAPHORES)
	$(LD) $(LDFLAGS) -T kernel.ld -o $(KERNEL) $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(OBJECTS_DRIVERS) $(OBJECTS_INTERRUPTIONS) $(OBJECTS_MMANAGER) $(OBJECTS_PROCESSES) $(OBJECTS_SEMAPHORES)
	$(LD) $(LDFLAGS) -T kernel.ld --oformat=elf64-x86-64 -o kernel.elf $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(OBJECTS_DRIVERS) $(OBJECTS_INTERRUPTIONS) $(OBJECTS_MMANAGER) $(OBJECTS_PROCESSES) $(OBJECTS_SEMAPHORES)

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

buddy:
	$(MAKE) BUDDY_MODE=1 all

clean:
	find . -name "*.o" -delete
	find . -name "*.elf" -delete
	find . -name "*.bin" -delete

.PHONY: all buddy clean
