include Makefile.inc

SAMPLE_DATA=0001-sampleDataModule.bin

all: sampleCodeModule sampleDataModule
buddy: sampleCodeModuleBuddy sampleDataModule

sampleCodeModule:
	cd SampleCodeModule; make

sampleCodeModuleBuddy:
	cd SampleCodeModule; make buddy

sampleDataModule:
	printf "This is sample data." >> $(SAMPLE_DATA) && dd if=/dev/zero bs=1 count=1 >> $(SAMPLE_DATA)

clean:
	cd SampleCodeModule; make clean
	find . -name "*.o" -delete
	find . -name "*.elf" -delete
	find . -name "*.bin" -delete


.PHONY: sampleCodeModule sampleCodeModuleBuddy all buddy clean
