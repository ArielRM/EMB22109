#ifndef COUNTER_HH_
#define COUNTER_HH_

#include <systemc>

using namespace sc_core;
using sc_dt::sc_uint;

template <unsigned BITS>
SC_MODULE(Counter)
{
	static_assert(BITS > 0);

	// Declare ports
	sc_in<bool> clock;			 /*!< Sinal de clock, sensível à borda de subida */
	sc_in<bool> reset;			 /*!< Reset assíncrono */
	sc_in<bool> clock_enable;	 /*!< Hsabilita o `clock` */
	sc_in<sc_uint<BITS>> preset; /*!< Valor colocado na saída quando `reset` é ativado */
	sc_out<sc_uint<BITS>> output;

	// Main process
	void process()
	{
		if (reset.read())
		{
			_count = preset.read();
		}
		else if (clock.read() && clock_enable.read())
		{
			_count++;
		}
		output.write(_count);
	}

	SC_CTOR(Counter) : _count(0)
	{
		SC_METHOD(process);
		sensitive << clock.pos() << reset.pos();
	}

	// Unit testbench
	static int testbench(const char *trace_file)
	{
		sc_trace_file *tf = sc_create_vcd_trace_file(trace_file);
		tf->set_time_unit(1, SC_NS);

		// Signals used in testbench
		sc_clock clock("clock", 2, SC_NS, 0.5);
		sc_signal<bool> reset, clock_enable;
		sc_signal<sc_uint<8>> preset, output;

		// Include signals to trace
		tf->trace(clock, "clock");
		tf->trace(reset, "reset");
		tf->trace(clock_enable, "enable");
		tf->trace(preset, "preset");
		tf->trace(output, "output");

		// Component initialization
		Counter counter("counter_tb");

		// Signal Connections
		counter.clock(clock);
		counter.reset(reset);
		counter.clock_enable(clock_enable);
		counter.preset(preset);
		counter.output(output);

		// Generate Stimuli
		reset = false;
		clock_enable = false;
		preset = 0;
		sc_start(2, SC_NS);
		clock_enable = true;
		sc_start(5, SC_NS);
		reset = true;
		sc_start(2, SC_NS);
		reset = false;
		sc_start(5, SC_NS);
		preset = 0xF0;
		reset = true;
		sc_start(2, SC_NS);
		reset = false;
		sc_start(5, SC_NS);
		clock_enable = false;

		// Close trace file
		sc_close_vcd_trace_file(tf);

		return 0;
	}

private:
	sc_uint<BITS> _count;
};

#endif /* COUNTER_H_ */
