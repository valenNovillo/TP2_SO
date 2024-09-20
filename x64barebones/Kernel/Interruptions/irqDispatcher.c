#include <stdint.h>
#include "../Drivers/include/timeDriver.h"
#include "../Drivers/include/keyBoardDriver.h"

static void int_20();
static void int_21();

void (*interrupt_handlers[])() = {int_20, int_21};


void irqDispatcher(uint64_t irq, uint64_t infoRegs) {
    if (irq >= sizeof(interrupt_handlers) / sizeof(interrupt_handlers[0])) 
            return;

    interrupt_handlers[irq](infoRegs);
}

void int_20() {
	timer_handler();
}

void int_21(uint64_t infoRegs) {
	keyboardHandler(infoRegs);
}