#ifndef ALU_HH_
#define ALU_HH_

#include <systemc>

using namespace sc_core;
using sc_dt::sc_int;
using sc_dt::sc_uint;

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
		switch (op.read())
		{
		case NOP:
			break;
		case ADD:
			_output = a.read() + b.read();
			break;
		case AND:
			_output = a.read() & b.read();
			break;
		case OR:
			_output = a.read() | b.read();
			break;
		case NOT:
			_output = ~b.read();
			break;
		case LDA:
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
		a = 1 << (BITS - 1);
		b = 1;

		op = ADD;
		sc_start(2, SC_NS);

		op = AND;
		sc_start(2, SC_NS);

		op = LDA;
		sc_start(2, SC_NS);

		op = NOP;
		sc_start(2, SC_NS);

		op = NOT;
		sc_start(2, SC_NS);

		op = OR;
		sc_start(2, SC_NS);

		sc_stop();

		// Close trace file
		sc_close_vcd_trace_file(tf);

		return 0;
	}

	static const char* operation_stringfy(unsigned int operation)
	{
		switch (operation)
		{
		case NOP:
			return("NOP");
		case ADD:
			return("ADD");
		case AND:
			return("AND");
		case OR:
			return("OR ");
		case NOT:
			return("NOT");
		case LDA:
			return("LDA");
		default:
			return("UNK");
			break;
		}
	}

	static const unsigned int NOP = 0;
	static const unsigned int ADD = 1;
	static const unsigned int AND = 2;
	static const unsigned int OR = 3;
	static const unsigned int NOT = 4;
	static const unsigned int LDA = 5;

private:
	sc_int<BITS> _output;
};

#endif /* ALU_HH_ */
