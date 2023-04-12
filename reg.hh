#ifndef REG_HH_
#define REG_HH_

#include <systemc>

using namespace sc_core;
using sc_dt::sc_int;

template <unsigned BITS>
SC_MODULE(Reg)
{
	static_assert(BITS > 0);

	// Declare ports
	sc_in<bool> clock, reset;
	sc_in<sc_int<BITS>> data;
	sc_out<sc_int<BITS>> output;

	// Main process
	void process()
	{
		if (reset)
		{
			output = 0;
		}
		else if (clock)
		{
			output = data;
		}
	}

	SC_CTOR(Reg)
	{
		SC_METHOD(process);
		sensitive_pos << clock << reset;
	}

	// Unit testbench
	static int testbench(const char *trace_file)
	{
		sc_trace_file *tf = sc_create_vcd_trace_file(trace_file);
		tf->set_time_unit(1, SC_NS);

		// Signals used in testbench
		sc_clock clock("clk", 2, SC_NS, 0.5);
		sc_signal<bool> reset;
		sc_signal<sc_int<BITS>> data, output;

		// Include signals to trace
		tf->trace(clock, "clock");
		tf->trace(reset, "reset");
		tf->trace(data, "data");
		tf->trace(output, "output");

		// Component initialization
		Reg<BITS> reg("Register");

		// Signal Connections
		reg.clock(clock);
		reg.reset(reset);
		reg.data(data);
		reg.output(output);

		// Generate Stimuli
		reset = true;
		data = 1;
		sc_start(2, SC_NS);

		reset = false;
		sc_start(10, SC_NS);

		reset = true;
		data = 1 << (BITS - 1);
		sc_start(2, SC_NS);

		reset = false;
		sc_start(10, SC_NS);

		// Close trace file
		sc_close_vcd_trace_file(tf);

		return 0;
	}
};

#endif /* REG_HH_ */
