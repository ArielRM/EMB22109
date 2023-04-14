#ifndef NEANDER_HH_
#define NEANDER_HH_

#include <systemc>
#include <iostream>

#include "mux.hh"
#include "alu.hh"
#include "counter.hh"
#include "reg.hh"
#include "mem.hh"
#include "control.hh"

using namespace sc_core;
using sc_dt::sc_int;
using sc_dt::sc_uint;
using sc_dt::sc_bv;

using std::cout;
using std::endl;

// Top-level
SC_MODULE(Neander)
{
	// Input Port
	sc_in<bool> clock;

	void process()
	{
		_mem_data_out_int = static_cast<sc_int<8>>(_mem_data_out.read());
		_mem_data_out_uint =static_cast<sc_uint<8>>(_mem_data_out.read());
		_mem_data_in = static_cast<sc_bv<8>>(_ac_data_out.read());
	}

	Mem<8, 8> *mem()
	{
		return &_mem;
	}

	void execution_stats()
	{
		cout << "++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
		cout << "Instructions Executed: " << _uc.instruction_count() << endl;
		cout << "               Cycles: " << _uc.cycles() << endl;
		cout << "                  CPI: " << _uc.cycles() / (double)_uc.instruction_count() << endl;
		cout << "++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
	}

	SC_CTOR(Neander) : _ac("AC"), _pc("PC"), _mux_addr("MUX_Addr"),
					   _mem("MEM"), _alu("ALU"), _uc("UC")
	{
		// Connect Signals
		_ac.clock(_ac_load);
		_ac.reset(_sempre_false);
		_ac.data(_ac_data_in);
		_ac.output(_ac_data_out);

		_pc.clock(clock);
		_pc.reset(_pc_load);
		_pc.clock_enable(_pc_inc);
		_pc.preset(_mem_data_out_uint);
		_pc.output(_pc_data_out);

		_mux_addr.select(_mem_addr_sel);
		_mux_addr.a(_pc_data_out);
		_mux_addr.b(_mem_data_out_uint);
		_mux_addr.output(_mem_addr_in);

		_mem.write(_mem_wr);
		_mem.read(_mem_rd);
		_mem.clock(clock);
		_mem.addr(_mem_addr_in);
		_mem.data_in(_mem_data_in);
		_mem.data_out(_mem_data_out);

		_alu.a(_mem_data_out_int);
		_alu.b(_ac_data_out);
		_alu.output(_ac_data_in);
		_alu.N(_ula_flagN);
		_alu.Z(_ula_flagZ);
		_alu.op(_ula_op);

		_uc.clock(clock);
		_uc.ula_flagN(_ula_flagN);
		_uc.ula_flagZ(_ula_flagZ);
		_uc.instruction_decode(_instruction_decode);
		_uc.mem_read(_mem_rd);
		_uc.pc_incr(_pc_inc);
		_uc.ac_load(_ac_load);
		_uc.mem_addr_src(_mem_addr_sel);
		_uc.mem_write(_mem_wr);
		_uc.pc_load(_pc_load);
		_uc.ula_op(_ula_op);

		// Declare process
		SC_METHOD(process);

		sensitive << _mem_data_out << _ac_data_out;
	}

	// Unit testbench
	static int testbench(const char *trace_file)
	{
		sc_trace_file *tf = sc_create_vcd_trace_file(trace_file);
		tf->set_time_unit(1, SC_NS);


		// Signals used in testbench
		sc_clock clk("clock", 2, SC_NS, -0.5);

		// Include signals to trace
		tf->trace(clk, "clock");

		// Component initialization
		Neander neander("Neander");

		// Signal Connections
		neander.clock(clk);

		// Generate Stimuli
		sc_start(10, SC_NS);

		// Close trace file
		sc_close_vcd_trace_file(tf);

		return 0;
	}

private:
	// Internal Components
	Reg<8> _ac;
	Counter<8> _pc;
	Mux<8> _mux_addr;
	Mem<8, 8> _mem;
	ALU<8> _alu;
	Control _uc;

	// Internal Signals
	sc_signal<sc_uint<8>> _mem_addr_in;
	sc_signal<sc_int<8>> _ac_data_out;
	sc_signal<sc_int<8>> _ac_data_in;
	sc_signal<sc_bv<8>> _mem_data_in;
	sc_signal<sc_bv<8>> _mem_data_out;
	sc_signal<sc_uint<8>> _pc_data_out;
	sc_signal<bool> _sempre_false;

	sc_signal<sc_uint<8>> _mem_data_out_uint; // esse existe porque o tipo de sinal precisa ser convertido
	sc_signal<sc_int<8>> _mem_data_out_int; // esse existe porque o tipo de sinal precisa ser convertido
	
	/* sinais de controle */
	sc_signal<bool> _mem_rd;
	sc_signal<bool> _mem_wr;
	sc_signal<bool> _mem_addr_sel;
	sc_signal<bool> _pc_inc;
	sc_signal<bool> _pc_load;
	sc_signal<bool> _ac_load;
	sc_signal<sc_uint<3>> _ula_op;

	sc_signal<bool> _ula_flagN;
	sc_signal<bool> _ula_flagZ;
	sc_signal<sc_uint<8>> _instruction_decode;
};

#endif /* NEANDER_HH_ */