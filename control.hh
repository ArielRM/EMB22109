
#ifndef CONTROL_H_
#define CONTROL_H_

#include <systemc.h>

SC_MODULE(Control){
    // Declare ports
    
    // Main process
	void process(){

	}

	SC_CTOR(Control){

	}
	
	unsigned int cycles() { return _cycles; }
	unsigned int instruction_count() { return _inst_count; }

private:
    // States Declaration
    
	// Internal Signals
	
	unsigned int			_cycles;
	unsigned int			_inst_count;
};

#endif /* CONTROL_H_ */