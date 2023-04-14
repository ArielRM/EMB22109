#ifndef MEM_HH_
#define MEM_HH_

#include <systemc>
#include <iostream>

using namespace sc_core;
using sc_dt::sc_bv;
using sc_dt::sc_uint;

template <unsigned BITS, unsigned ADDR_BITS>
SC_MODULE(Mem)
{
	static_assert(BITS > 0);
	static_assert(ADDR_BITS > 0 && ADDR_BITS < UINT_MAX);

	// Declare ports
	sc_in<bool> write, read, clock;
	sc_in<sc_uint<ADDR_BITS>> addr;
	sc_in<sc_bv<BITS>> data_in;
	sc_out<sc_bv<BITS>> data_out;

	// Main process
	void process()
	{
		if (write)
		{
			_memory[addr.read()] = data_in;
		}

		if (read)
		{
			data_out = _memory[addr.read()];
		}
	}

	SC_CTOR(Mem) : _memory()
	{
		SC_METHOD(process);
		sensitive_pos << clock;
	}

	// For trace/tests purpouse. Prints memory contents no stdout
	void dump_mem(unsigned int start = 0, unsigned int end = (1 << ADDR_BITS) - 1)
	{
		for (unsigned i = start; i <= end; i++)
		{
			std::cout << i << " - " << _memory[i] << std::endl;
		}
	}

	// Unit testbench
	static int testbench(const char *trace_file)
	{
		sc_trace_file *tf = sc_create_vcd_trace_file(trace_file);
		tf->set_time_unit(1, SC_NS);

		// Signals used in testbench
		sc_clock clock("clock", 2, SC_NS, 0.5);
		sc_signal<bool> write, read;
		sc_signal<sc_uint<8>> addr;
		sc_signal<sc_bv<8>> data_in;
		sc_signal<sc_bv<8>> data_out;

		// Include signals to trace
		tf->trace(clock, "clock");
		tf->trace(write, "write");
		tf->trace(read, "read");
		tf->trace(addr, "addr");
		tf->trace(data_in, "in");
		tf->trace(data_out, "out");

		// Component initialization
		Mem<BITS, ADDR_BITS> mem("memory");

		// Signal Connections
		mem.clock(clock);
		mem.write(write);
		mem.read(read);
		mem.addr(addr);
		mem.data_in(data_in);
		mem.data_out(data_out);

		// Generate Stimuli

		// não faz nada
		write = false;
		read = false;
		data_in = 1 << (BITS - 1);
		addr = 0;
		sc_start(2, SC_NS);

		// escreve 170 em 0x00
		write = true;
		data_in = 1 << (BITS - 2);
		addr = 0;
		sc_start(2, SC_NS);

		// escreve 85 em 0x03
		addr = 3;
		data_in = 85;
		sc_start(2, SC_NS);

		// lê de 0x00
		write = false;
		read = true;
		addr = 0;
		sc_start(2, SC_NS);

		// lê de 0x03
		read = true;
		addr = 3;
		sc_start(2, SC_NS);

		// não faz nada
		write = false;
		read = false;
		data_in = 0XFF;
		addr = 0XFF;
		sc_start(2, SC_NS);
		// Close trace file
		sc_close_vcd_trace_file(tf);

		return 0;
	}

	sc_bv<BITS> _memory[(1 << ADDR_BITS) - 1];
private:
};

#endif /* MEM_HH_ */
