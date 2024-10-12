#include <stdint.h>
#include "include/lib.h"
#include "include/moduleLoader.h"
#include "Interruptions/include/idtLoader.h"
#include "include/memoryManager.h"

#include "Test/test_mm.h"
#include "include/lib.h"
#include "include/processes.h"
#include "include/stack.h"
#include "include/typedef_process.h"

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;


static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;
static void * const stacks = (void*)0x600000;
static void * const startFreeMemoryAddress = (void*)0x700000; // 4KB stacks, 256 total stacks, 100000 adresses

typedef int (*EntryPoint)();


void clearBSS(void * bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary()
{
	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);
	clearBSS(&bss, &endOfKernel - &bss);
	
	return getStackBase();
}

int main()
{	
	load_idt();
	my_mm_init(startFreeMemoryAddress);
	stack_init(stacks);

	//char* param[1];
	//param[0] = MEMORY_SIZE;
    //test_mm(1, param);
	
	((EntryPoint)sampleCodeModuleAddress)();

	return 0;
}