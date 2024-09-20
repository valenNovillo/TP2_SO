include ../Makefile.inc

MODULE=0000-sampleCodeModule.bin
SOURCES=$(wildcard [^_]*.c)
SOURCES_ASM=$(wildcard ../asm/*.asm)
SOURCES_SHELL=$(wildcard Shell/*c)
SOURCES_LIBS=$(wildcard Library/*c)
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_SHELL=$(SOURCES_SHELL:.c=.o)
OBJECTS_LIBS=$(SOURCES_LIBS:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
LOADERSRC=_loader.c

LOADEROBJECT=$(LOADERSRC:.c=.o)

all: $(MODULE)

$(MODULE): $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_SHELL) $(OBJECTS_LIBS) $(OBJECTS_ASM)
	$(LD) $(LDFLAGS) -T sampleCodeModule.ld -o ../$(MODULE) $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_SHELL) $(OBJECTS_LIBS) $(OBJECTS_ASM)	

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(GCC) $(GCCFLAGS) $(LOADERSRC) -c -o $(LOADEROBJECT)

clean:
	rm -rf asm/*.o *.o

.PHONY: all clean