#ifndef ALU_HH_
#define ALU_HH_

#include <systemc>
#include <iostream>

using namespace sc_core;
using sc_dt::sc_int;
using sc_dt::sc_uint;

#include "alu_op.hh"

template <unsigned BITS>
SC_MODULE(ALU)
{	
	static_assert(BITS > 0);
	
	// Declare ports
	sc_in<sc_int<BITS>> a, b;
	sc_in<sc_uint<3>> op;
	sc_out<bool> N, Z;
	sc_out<sc_int<BITS>> output;

	// Main process
	void process()
	{
		unsigned temp = op.read();
		ALUOperation operation = static_cast<ALUOperation>(temp);

		switch (operation)
		{
		case ALUOperation::NOP:
			break;
		case ALUOperation::ADD:
			_output = a.read() + b.read();
			break;
		case ALUOperation::AND:
			_output = a.read() & b.read();
			break;
		case ALUOperation::OR:
			_output = a.read() | b.read();
			break;
		case ALUOperation::NOT:
			_output = ~b.read();
			break;
		case ALUOperation::LDA:
			_output = a.read();
			break;
		default:
			break;
		}

		output = _output;
		N = _output < 0;
		Z = _output == 0;
	}

	SC_CTOR(ALU) : _output()
	{
		SC_METHOD(process);
		sensitive << a << b << op;
	}

	// Unit testbench
	static int testbench(const char *trace_file)
	{
		sc_trace_file *tf = sc_create_vcd_trace_file(trace_file);
		tf->set_time_unit(1, SC_NS);

		// Signals used in testbench
		sc_signal<sc_int<BITS>> a, b, output;
		sc_signal<sc_uint<3>> op;
		sc_signal<bool> N, Z;
		sc_signal<std::string> ops;

		// Include signals to trace
		tf->trace(a, "a");
		tf->trace(b, "b");
		tf->trace(output, "output");
		tf->trace(op, "operation");
		tf->trace(N, "N");
		tf->trace(Z, "Z");

		// Component initialization
		ALU<BITS> ula("ULA");

		// Signal Connections
		ula.a(a);
		ula.b(b);
		ula.output(output);
		ula.op(op);
		ula.N(N);
		ula.Z(Z);

		// Generate Stimuli
		ALUOperation op_list[] = {ALUOperation::ADD};
		
		a = 1 << (BITS - 1);
		b = 1;

		op = op_list[0];
		sc_start(2, SC_NS);

		op = ALUOperation::AND;
		sc_start(2, SC_NS);

		op = ALUOperation::LDA;
		sc_start(2, SC_NS);

		op = ALUOperation::NOP;
		sc_start(2, SC_NS);

		op = ALUOperation::NOT;
		sc_start(2, SC_NS);

		op = ALUOperation::OR;
		sc_start(2, SC_NS);

		sc_stop();

		// Close trace file
		sc_close_vcd_trace_file(tf);

		return 0;
	}

private:
	sc_int<BITS> _output;
};

#endif /* ALU_HH_ */
