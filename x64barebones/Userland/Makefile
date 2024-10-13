include Makefile.inc

SAMPLE_DATA=0001-sampleDataModule.bin

all: sampleCodeModule sampleDataModule

sampleCodeModule:
	cd SampleCodeModule; make

sampleDataModule:
	printf "This is sample data." >> $(SAMPLE_DATA) && dd if=/dev/zero bs=1 count=1 >> $(SAMPLE_DATA)

clean:
	cd SampleCodeModule; make clean
#	rm -rf *.bin *.elf asm/*.o
	find . -name "*.o" -delete
	find . -name "*.elf" -delete
	find . -name "*.bin" -delete


.PHONY: sampleCodeModule all clean
