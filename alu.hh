
#ifndef ALU_H_
#define ALU_H_

#include <systemc.h>

SC_MODULE(ALU){
    // Declare ports
    
    // Main process
	void process(){

	}

	SC_CTOR(ALU){

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

#endif /* ALU_H_ */
