#include <stdint.h>
#include "include/lib.h"
#include "include/moduleLoader.h"
#include "Interruptions/include/idtLoader.h"
#include "include/memoryManager.h"

#include "include/lib.h"
#include "include/processes.h"
#include "include/stack.h"
#include "include/scheduler.h"
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

int main() {	
	my_mm_init(startFreeMemoryAddress);
	stack_init(stacks);
	initialize_scheduler();
	char * argv[] = {NULL};
	int16_t fds[3] = {STDIN, STDOUT, STDERR};
	create_process(sampleCodeModuleAddress, argv, "default", 0, fds);
	load_idt();
	call_timer_tick();
	
	//((EntryPoint)sampleCodeModuleAddress)();

	return 0;
}