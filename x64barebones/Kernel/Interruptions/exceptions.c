#include "include/exceptions.h"

#include "../Drivers/include/videoDriver.h"
#include "../Drivers/include/timeDriver.h"
#include "../Drivers/include/keyBoardDriver.h"

static void zero_division(){
	putString(2, "\nZero Division Exception.\nCurrent state of registers:\n\n", 55);
}

static void invalid_opcode(){
    putString(2, "\nInvalid Opcode Exception.\nCurrent state of registers:\n\n",56);
}

static void (*exception_handlers[])() = {zero_division, invalid_opcode};

void exceptionDispatcher(int exception, uint64_t infoRegs){
	if (exception >= sizeof(exception_handlers) / sizeof(exception_handlers[0])) 
            return;
	
	setReading(0);
	cleanScreen();

	REGISTERS * aux = (REGISTERS *) infoRegs; //cambiamos el formato de los registros a imprimir
	exception_handlers[exception]();

	printRegs(*aux); //Imprimimos los registros

	putString(2, "Press ESC to restart the system.\n", 33);

	while (keyPress() != 1) {
		_hlt();
	}

	cleanScreen();

	return;

}
