#include "neander.hh"

using std::cout;

template <typename T, unsigned N>
static constexpr unsigned int to_uint(T signal) {
	return static_cast<unsigned>(static_cast<sc_uint<N>>(signal));
}

void Neander::draw() {
	char pc_data_out[] = "0xFF";
	char mem_addr_in[] = "0xFF";
	char mem_data_out[] = "-AAA";
	char mux_in[] = "0xFF";
	char operation[] = "UND (FF)";
	char mem_data_in[] = "-BBB";
	char ac_data_in[] = "-255";
	char ula_op[] = "UND(0)";

	std::snprintf(pc_data_out, 5, "0x%02X", static_cast<unsigned>(_pc_data_out.read()));
	std::snprintf(mem_addr_in, 5, "0x%02X", static_cast<unsigned>(_mem_addr_in.read()));
	std::snprintf(mem_data_out, 5, "%.4i", static_cast<int>(static_cast<sc_int<8>>(_mem_data_out.read())));
	std::snprintf(mux_in, 5, "0x%02X", static_cast<unsigned>(static_cast<sc_uint<8>>(_mem_data_out.read())));

	unsigned int isa = static_cast<unsigned>(static_cast<sc_uint<8>>(_mem_data_out.read()));
	std::snprintf(operation, 9, "%3s (%02X)", Control::instruction_stringfy(isa), isa);

	std::snprintf(mem_data_in, 5, "%.4i", static_cast<int>(static_cast<sc_int<8>>(_mem_data_in.read())));
	std::snprintf(ac_data_in, 5, "%.4i", static_cast<int>(_ac_data_in.read()));

	unsigned int op = static_cast<unsigned>(_ula_op.read());
	std::snprintf(ula_op, 7, "%s(%1.1i)", ALU<8>::operation_stringfy(op), op);

cout << "  ┌┄╴mem_rd = " << _mem_rd <<                     "╶┄┐                                        " <<             "     " << endl;
cout << " ┌┴───────────" <<                               "───┴────────────────────────────────────────" <<             "───┐ " << endl;
cout << " │ ┌┄╴mem_wr = " << _mem_wr <<                     "╶┄┐                                       " <<             "   │ " << endl;
cout << " │┌┴───────────" <<                               "───┴───────────────────────────────────────" <<             "─┐ │ " << endl;
cout << " ││ ┌┄╴mem_addr_sel = " << _mem_addr_sel <<               "╶┄┐                                " <<             " │ │ " << endl;
cout << " ││┌┴─────────────────" <<                               "───┴──────────────┐                 " <<             " │ │ " << endl;
cout << " │││ ┌┄╴pc_load = " << _pc_load <<   "╶┄┐┌───┐┌┄╴" << pc_data_out << "╶┄┐ ┌─┴─┐               " <<             "┌┴─┴┐" << endl;
cout << " │││┌┴────────────" <<              "───┴┤   ┝┷━━" <<            "━━━━━━┷━┥ M │               " <<             "│ M │" << endl;
cout << " ││││ ┌┄╴pc_inc = " << _pc_inc <<    "╶┄┐│ P │   " <<            "        │   │┌┄╴" << mem_addr_in <<  "╶┄┐     │   │" << endl;
cout << " ││││┌┴───────────" <<              "───┴┤   │   " <<            "        │ U ┝┷━━" <<             "━━━━━━┷━━━━━┥ E │" << endl;
cout << " │││││ ┌┄╴clock = " << clock      <<   "╶┄┐│ C │ ┌┄╴" << mux_in <<      "╶┄┐│   │     ┌┄╴" << mem_data_out << "╶┄┐│   │" << endl;
cout << " │││││┌┴──────────" <<              "───┴┤   │┏┷━━" <<            "━━━━━━┷┥ X │   ┏━┷━━" <<             "━━━━━━┷┥ M │" << endl;
cout << " ││││││ ┌┄╴" << operation <<         "╶┄┐└─┰─┘┃   " <<            "       └───┘   ┃    " <<             "       └┰─┬┘" << endl;
cout << " ││││││┏┷━━" <<              "━━━━━━━━━━┷━━┻━━┻━━━━┳━━━" <<            "━━━━━━━━━━┛  ┌┄╴" << mem_data_in <<  "╶┄┐┃ │ " << endl;
cout << " ││││││┃   " <<              "                     ┃ ┏━" <<            "━━━━━━━━━━━━┳┷━━" <<             "━━━━━━┷┛ │ " << endl;
cout << "┌┴┴┴┴┴┴┸┐┌┄╴ula_flagN = " << _ula_flagN << "╶┄┐   ┌┸─┸┐┌┄╴" << ac_data_in <<  "╶┄┐┌─┸─┐ " <<             "         │ " << endl;
cout << "│       ├┴──────────────" <<              "───┴───┤ U ┝┷━━" <<            "━━━━━━┷┥ A │ " <<             "         │ " << endl;
cout << "│       │┌┄╴ula_flagZ = " << _ula_flagZ << "╶┄┐   │   │   " <<            "       │   │ " <<             "         │ " << endl;
cout << "│   U   ├┴──────────────" <<              "───┴───┤ L │   " <<            "       │ C │ " <<             "         │ " << endl;
cout << "│       │┌┄╴ula_op = " << ula_op <<          "╶┄┐ │   │   " <<            "       │   │ " <<             "         │ " << endl;
cout << "│       ┝┷━━━━━━━━━━━" <<              "━━━━━━━━┷━┥ A │  ┌" <<            "───────┤ C │ " <<             "         │ " << endl;
cout << "│   C   │┌┄╴ac_load = " << _ac_load <<   "╶┄┐     └───┘  │" <<            "       └───┘ " <<             "         │ " << endl;
cout << "│       ├┴────────────" <<              "───┴────────────┘" <<            "       ┌┄╴clock = " << clock <<       "╶┄┐│ " << endl;
cout << "│       ├──────────────" <<                               "───────────────────────┴──────────" <<             "───┴┘ " << endl;
cout << "└───────┘                                                      "                                                       << endl;

//		cout << "  ┌┄╴mem_rd = 0╶┄┐                                             " << endl;
//		cout << " ┌┴──────────────┴───────────────────────────────────────────┐ " << endl;
//		cout << " │ ┌┄╴mem_wr = 0╶┄┐                                          │ " << endl;
//		cout << " │┌┴──────────────┴────────────────────────────────────────┐ │ " << endl;
//		cout << " ││ ┌┄╴mem_addr_sel = 0╶┄┐                                 │ │ " << endl;
//		cout << " ││┌┴────────────────────┴──────────────┐                  │ │ " << endl;
//		cout << " │││ ┌┄╴pc_load = 0╶┄┐┌───┐┌┄╴0xFF╶┄┐ ┌─┴─┐               ┌┴─┴┐" << endl;
//		cout << " │││┌┴───────────────┴┤   ┝┷━━━━━━━━┷━┥ M │               │ M │" << endl;
//		cout << " ││││ ┌┄╴pc_inc = 0╶┄┐│ P │           │   │┌┄╴0xFF╶┄┐     │   │" << endl;
//		cout << " ││││┌┴──────────────┴┤   │           │ U ┝┷━━━━━━━━┷━━━━━┥ E │" << endl;
//		cout << " │││││ ┌┄╴clock = 0╶┄┐│ C │ ┌┄╴0xFF╶┄┐│   │     ┌┄╴-128╶┄┐│   │" << endl;
//		cout << " │││││┌┴─────────────┴┤   │┏┷━━━━━━━━┷┥ X │   ┏━┷━━━━━━━━┷┥ M │" << endl;
//		cout << " ││││││ ┌┄╴UND (AB)╶┄┐└─┰─┘┃          └───┘   ┃           └┰─┬┘" << endl;
//		cout << " ││││││┏┷━━━━━━━━━━━━┷━━┻━━┻━━━━┳━━━━━━━━━━━━━┛  ┌┄╴+123╶┄┐┃ │ " << endl;
//		cout << " ││││││┃                        ┃ ┏━━━━━━━━━━━━━┳┷━━━━━━━━┷┛ │ " << endl;
//		cout << "┌┴┴┴┴┴┴┸┐┌┄╴ula_flagN = 0╶┄┐   ┌┸─┸┐┌┄╴-001╶┄┐┌─┸─┐          │ " << endl;
//		cout << "│       ├┴─────────────────┴───┤ U ┝┷━━━━━━━━┷┥ A │          │ " << endl;
//      cout << "│       │┌┄╴ula_flagZ = 0╶┄┐   │   │          │   │          │ " << endl;
//		cout << "│   U   ├┴─────────────────┴───┤ L │          │ C │          │ " << endl;
//      cout << "│       │┌┄╴ula_op = UOP(5)╶┄┐ │   │          │   │          │ " << endl;
//		cout << "│       ┝┷━━━━━━━━━━━━━━━━━━━┷━┥ A │  ┌───────┤ C │          │ " << endl;
//		cout << "│   C   │┌┄╴ac_load = 0╶┄┐     └───┘  │       └───┘          │ " << endl;
//		cout << "│       ├┴───────────────┴────────────┘       ┌┄╴clock = 0╶┄┐│ " << endl;
//		cout << "│       ├─────────────────────────────────────┴─────────────┴┘ " << endl;
//		cout << "└───────┘                                                      " << endl;

}

