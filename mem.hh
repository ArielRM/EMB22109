#ifndef MEM_H_
#define MEM_H_

#include <systemc.h>
#include <iomanip>

SC_MODULE(Mem){
    // Declare ports
    
    // Main process
	void process(){

	}

	SC_CTOR(Mem){

	}
	
	//For trace/tests purpouse. Prints memory contents no stdout
	void dump_mem(unsigned int start = 0, unsigned int end = 256){
		
	}

    // Unit testbench
	static int testbench(const char * trace_file){
	    sc_trace_file *tf = sc_create_vcd_trace_file(trace_file);
	    
	    //Signals used in testbench
		
        //Include signals to trace
	  	
        //Component initialization

		// Signal Connections

		// Generate Stimuli
		
        // Close trace file
	  	sc_close_vcd_trace_file(tf);

	  	return 0;
	}
};

#endif /* MEM_H_ */