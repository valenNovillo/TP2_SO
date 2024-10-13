include ../Makefile.inc

MODULE=0000-sampleCodeModule.bin
SOURCES=$(wildcard [^_]*.c)
SOURCES_ASM=$(wildcard ../asm/*.asm)
SOURCES_SHELL=$(wildcard Shell/*.c)
SOURCES_LIBS=$(wildcard Library/*.c)
SOURCES_TESTS=$(wildcard Test/*.c)
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_SHELL=$(SOURCES_SHELL:.c=.o)
OBJECTS_LIBS=$(SOURCES_LIBS:.c=.o)
OBJECTS_TESTS=$(SOURCES_TESTS:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
LOADERSRC=_loader.c

LOADEROBJECT=$(LOADERSRC:.c=.o)

all: $(MODULE)

$(MODULE): $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_SHELL) $(OBJECTS_LIBS) $(OBJECTS_TESTS) $(OBJECTS_ASM)
	$(LD) $(LDFLAGS) -T sampleCodeModule.ld -o ../$(MODULE) $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_SHELL) $(OBJECTS_LIBS) $(OBJECTS_TESTS) $(OBJECTS_ASM)
	$(LD) $(LDFLAGS) -T sampleCodeModule.ld --oformat=elf64-x86-64 -o ../sampleCodeModule.elf $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_SHELL) $(OBJECTS_LIBS) $(OBJECTS_TESTS) $(OBJECTS_ASM)	

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(GCC) $(GCCFLAGS) $(LOADERSRC) -c -o $(LOADEROBJECT)

clean:
#	rm -rf Shell/*.o Library/*.o *.o
	find . -name "*.o" -delete

.PHONY: all clean