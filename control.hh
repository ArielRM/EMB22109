#ifndef CONTROL_HH_
#define CONTROL_HH_

#include <systemc>

using namespace sc_core;
using sc_dt::sc_uint;

SC_MODULE(Control)
{
	// Declare ports
	sc_in<bool> clock;
	sc_in<bool> ula_flagN;
	sc_in<bool> ula_flagZ;
	sc_in<sc_uint<8>> instruction_decode;

	sc_out<bool> mem_read;
	sc_out<bool> pc_incr;
	sc_out<bool> ac_load;
	sc_out<bool> mem_addr_src;
	sc_out<bool> mem_write;
	sc_out<bool> pc_load;
	sc_out<sc_uint<3>> ula_op;

	// Main process
	void process()
	{
		switch (_state.read())
		{
		case CS_FETCH_INST:
			mem_read = true;
			pc_incr = true;
			ac_load = false;
			mem_addr_src = false;
			mem_write = false;
			pc_load = false;
			break;
		case CS_MEM_READ_INST:
			mem_read = false;
			pc_incr = false;
			_instruction = instruction_decode;
			break;
		case CS_DECODE_INST:
			break;
		case CS_LOAD_OP_ADDR:
			mem_read = true;
			pc_incr = true;
			break;
		case CS_LOAD_OP_DATA:
			mem_addr_src = true;
			mem_read = true;
			pc_incr = false;
			break;
		case CS_MEM_READ_DATA:
			mem_addr_src = false;
			mem_read = false;
			break;
		case CS_MEM_WRITE:
			mem_addr_src = true;
			mem_write = true;
			pc_incr = false;
			break;
		case CS_JMP_EXEC:
			pc_incr = false;
			switch (_instruction)
			{
			case JMP:
				pc_load = true;
				break;
			case JN:
				if (_n_flag)
				{
					pc_load = true;
				}
				break;
			case JZ:
				if (_z_flag)
				{
					pc_load = true;
				}
				break;
			case NOP:
			case HLT:
			case NOT:
			case STA:
			case ADD:
			case AND:
			case OR:
			case LDA:
			default:
				_state = State::CS_HLT; // should not reach
				break;
			}
			break;
		case CS_ALU_AC_WRITE:
			_n_flag = ula_flagN;
			_z_flag = ula_flagZ;
			ac_load = true;
			break;
		case CS_ALU_EXEC:
			pc_incr = false;
			ula_op = alu_op(_instruction);
			break;
		case CS_HLT:
			break;
		}
	}

	void next_state()
	{
		if (clock.read())
		{
			switch (_state.read())
			{
			case CS_FETCH_INST:
				_state = CS_MEM_READ_INST;
				break;
			case CS_MEM_READ_INST:
				_state = CS_DECODE_INST;
				break;
			case CS_DECODE_INST:
				//std::cout << "INST: " << instruction_stringfy(_instruction) << " - " << _instruction << std::endl;
				switch (_instruction)
				{
				case NOP:
					end_instruction();
					break;
				case HLT:
					_state = CS_HLT;
					break;
				case NOT:
					_state = CS_ALU_EXEC;
					break;
				case STA:
				case ADD:
				case AND:
				case OR:
				case LDA:
				case JMP:
				case JN:
				case JZ:
				default:
					if (has_op(_instruction))
					{
						_state = CS_LOAD_OP_ADDR;
					}
				}
				break;
			case CS_LOAD_OP_ADDR:
				switch (_instruction)
				{
				case STA:
					_state = CS_MEM_WRITE;
					break;
				case JMP:
				case JN:
				case JZ:
					_state = CS_JMP_EXEC;
					break;
				case ADD:
				case AND:
				case OR:
				case LDA:
					_state = CS_LOAD_OP_DATA;
					break;
				default: // should not reach
					_state = CS_HLT;
					break;
				}
				break;
			case CS_LOAD_OP_DATA:
				_state = CS_MEM_READ_DATA;
				break;
			case CS_MEM_READ_DATA:
				_state = CS_ALU_EXEC;
				break;
			case CS_MEM_WRITE:
				end_instruction();
				break;
			case CS_JMP_EXEC:
				end_instruction();
				break;
			case CS_ALU_AC_WRITE:
				end_instruction();
				break;
			case CS_ALU_EXEC:
				_state = CS_ALU_AC_WRITE;
				break;
			case CS_HLT:
				break;
			}
			_cycles++;
		}
		else // Inicialização
		{
			_state = State::CS_FETCH_INST;
		}
	}

	SC_CTOR(Control)
	{
		SC_METHOD(next_state);
		sensitive_pos << clock;

		SC_METHOD(process);
		sensitive << _state
				  << instruction_decode;
	}

	unsigned int cycles()
	{
		return _cycles;
	}
	unsigned int instruction_count()
	{
		return _inst_count;
	}

	static const char *instruction_stringfy(unsigned instruction)
	{
		switch (instruction)
		{
		case NOP:
			return ("NOP");
		case NOT:
			return ("NOT");
		case HLT:
			return ("HLT");
		case STA:
			return ("STA");
		case ADD:
			return ("ADD");
		case AND:
			return ("AND");
		case OR:
			return ("OR ");
		case LDA:
			return ("LDA");
		case JMP:
			return ("JMP");
		case JN:
			return ("JN ");
		case JZ:
			return ("JZ ");
		default:
			return ("UNK");
		}
	}

	const char *get_state_str()
	{
		switch (_state.read())
		{
		case CS_FETCH_INST:
			return ("FETCH_INST");
			break;
		case CS_MEM_READ_INST:
			return ("MEM_READ_INST");
			break;
		case CS_DECODE_INST:
			return ("DECODE_INST");
			break;
		case CS_LOAD_OP_ADDR:
			return ("LOAD_OP_ADDR");
			break;
		case CS_LOAD_OP_DATA:
			return ("LOAD_OP_DATA");
			break;
		case CS_MEM_READ_DATA:
			return ("MEM_READ_DATA");
			break;
		case CS_MEM_WRITE:
			return ("MEM_WRITE");
			break;
		case CS_JMP_EXEC:
			return ("JMP_EXEC");
			break;
		case CS_ALU_AC_WRITE:
			return ("ALU_AC_WRITE");
			break;
		case CS_ALU_EXEC:
			return ("ALU_EXEC");
			break;
		case CS_HLT:
			return ("HLT");
			break;
		}
	}

	static const unsigned int NOP = 0b0'000'0'000;
	static const unsigned int NOT = 0b0'100'0'000;
	static const unsigned int HLT = 0b0'111'0'000;
	static const unsigned int STA = 0b1'000'0'000;
	static const unsigned int ADD = 0b1'001'0'000;
	static const unsigned int AND = 0b1'010'0'000;
	static const unsigned int OR = 0b1'011'0'000;
	static const unsigned int LDA = 0b1'101'0'000;
	static const unsigned int JMP = 0b1'110'0'000;
	static const unsigned int JN = 0b1'111'1'000;
	static const unsigned int JZ = 0b1'111'0'000;

private:
	static unsigned int alu_op(const unsigned int inst)
	{
		return (inst & 0b0'111'0'000) >> 4;
	}

	static bool has_op(const unsigned int inst)
	{
		return inst & 0b1'000'0'000;
	}

	void end_instruction()
	{
		_inst_count++;
		_state = State::CS_FETCH_INST;
	}

	// States Declaration
	enum State
	{
		CS_FETCH_INST = 0,
		CS_MEM_READ_INST = 1,
		CS_DECODE_INST = 2,
		CS_LOAD_OP_ADDR = 3,
		CS_LOAD_OP_DATA = 4,
		CS_MEM_READ_DATA = 5,
		CS_MEM_WRITE = 6,
		CS_JMP_EXEC = 7,
		CS_ALU_AC_WRITE = 8,
		CS_ALU_EXEC = 9,
		CS_HLT = 10,
	};
	// Internal Signals
	sc_signal<State> _state;

	sc_uint<8> _instruction;
	bool _n_flag;
	bool _z_flag;

	unsigned int _cycles;
	unsigned int _inst_count;
};

#endif /* CONTROL_HH_ */
