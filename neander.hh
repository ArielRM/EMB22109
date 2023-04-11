#ifndef NEANDER_H_
#define NEANDER_H_

#include <systemc>

#include "mux.h"
#include "alu.h"
#include "cnt.h"
#include "reg.h"
#include "mem.h"
#include "control.h"

// Top-level
SC_MODULE(Neander){
    // Input Port
	sc_in<bool>  clk;
	

    void process(){

	}

	Mem* mem(){
		return &_mem;
	}

	void execution_stats(){
		cout << "++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
		cout << "Instructions Executed: " << _uc.instruction_count() << endl;
		cout << "               Cycles: " << _uc.cycles() << endl;
		cout << "                  CPI: " << _uc.cycles()/(double)_uc.instruction_count() << endl;
		cout << "++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
	}

	SC_CTOR(Neander):
		_ac("AC"), _pc("PC"), _mux_addr("MUX_Addr"),
		_mem("MEM"), _alu("ALU"), _uc("UC")
	{
		//Connect Signals
		
		
		//Declare process
		
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

private:
    // Internal Components
	Register 	_ac;
	Counter		_pc;
	Mux      	_mux_addr;
	Mem 		_mem;
	ULA			_alu;
	Control		_uc;
	
	// Internal Signals

};

#endif /* NEANDER_H_ */