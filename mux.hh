#ifndef MUX_HH_
#define MUX_HH_

#include <systemc>

using namespace sc_core;
using sc_dt::sc_uint;

template <unsigned BITS>
SC_MODULE(Mux)
{
	static_assert(BITS > 0);

	// Declare ports
	sc_in<bool> select;
	sc_in<sc_uint<BITS>> a, b;
	sc_out<sc_uint<BITS>> output;

	// Main process
	void process()
	{
		if (select)
		{
			output = b;
		}
		else
		{
			output = a;
		}
	}

	SC_CTOR(Mux)
	{
		SC_METHOD(process);
		sensitive << a << b << select;
	}

	// Unit testbench
	static int testbench(const char *trace_file)
	{
		sc_trace_file *tf = sc_create_vcd_trace_file(trace_file);
		tf->set_time_unit(1, SC_NS);

		// Signals used in testbench
		sc_signal<bool> select;
		sc_signal<sc_uint<BITS>> a, b, output;

		// Include signals to trace
		tf->trace(select, "select");
		tf->trace(a, "a");
		tf->trace(b, "b");
		tf->trace(output, "output");

		// Component initialization
		Mux<BITS> mux("Mux");

		// Signal Connections
		mux.select(select);
		mux.a(a);
		mux.b(b);
		mux.output(output);

		// Generate Stimuli
		a = 1;
		b = 1 << (BITS - 1);
		select = false;
		sc_start(1, SC_NS);

		// A saída deve alternar entre `a` e `b`
		select = true;
		sc_start(1, SC_NS);
		select = false;
		sc_start(2, SC_NS);

		// A saída deve ser alterada junto com `a`
		a = 0;
		b = 1 << (BITS - 1);
		sc_start(2, SC_NS);

		// A saída deve alternar para `b`
		select = true;
		sc_start(2, SC_NS);

		// A saída deve ser alterada junto com `b`
		a = 1 << (BITS - 1);
		b = 0;
		sc_start(2, SC_NS);

		// Close trace file
		sc_close_vcd_trace_file(tf);

		return 0;
	}
};

#endif /* MUX_HH_ */