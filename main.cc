#include <systemc>
#include <iostream>
#include "mem.hh"

using namespace std;
using namespace sc_core;

int sc_main(int argc, char *argv[]) {
	sc_report_handler::set_verbosity_level(0);


	// Abaixo estão as rotinas de testes unitários de cada módulo.
	// Não é possível no systemC executá-las simultaneamente, desta
	// forma, apenas um dos testes deve ser descomentado por vez.

	cout << "Starting Simulation!" << endl;

//	Mux<8>::testbench("mux_trace");
//	Reg<8>::testbench("reg_trace");
//	Counter<8>::testbench("counter_trace");
//	ALU<8>::testbench("alu_trace");
//	Mem<8, 8>::testbench("mem_trace");

//	Neander::testbench("neander");

	cout << "Ending Simulation!" << endl;

	return 0;
}
