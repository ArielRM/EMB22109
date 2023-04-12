#ifndef CONTROL_H_
#define CONTROL_H_

#include <systemc>

using namespace sc_core;
using sc_dt::sc_uint;

enum ISA : unsigned
{
	NOP,
	NOT,
	HLT,
	STA,
	ADD,
	AND,
	OR,
	LDA,
	JMP,
	JN,
	JZ,
};

SC_MODULE(Control)
{
	// Declare ports
	sc_in<bool> clock;
	sc_in<bool> ula_flagN;
	sc_in<bool> ula_flagZ;
	sc_in<ISA> instruction_decode;

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
			break;
		case CS_MEM_READ_INST:
			mem_read = false;
			pc_incr = true;
			_instruction = instruction_decode;
			break;
		case CS_DECODE_INST:
			break;
		case CS_LOAD_OP_ADDR:
			mem_read = true;
			pc_incr = false;
			break;
		case CS_LOAD_OP_DATA:
			mem_addr_src = true;
			mem_read = true;
			pc_incr = false;
			break;
		case CS_MEM_READ_DATA:
			mem_addr_src = false;
			mem_read = true;
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
				if (_n_flag) {
					pc_load = true;
				}
				break;
			case JZ:
				if (_z_flag) {
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
				_state = State::CS_HLT;  // should not reach
				break;
			}
			break;
		case CS_ALU_AC_WRITE:
			ac_load = true;
			_n_flag = ula_flagN;
			_z_flag = ula_flagZ;
			break;
		case CS_ALU_EXEC:
			pc_incr = false;
			switch (_instruction)
			{
			case ADD:
				ula_op = 1;
				break;
			case AND:
				ula_op = 2;
				break;
			case OR:
				ula_op = 3;
				break;
			case NOT:
				ula_op = 4;
				break;
			case LDA:
				ula_op = 5;
				break;
			case JMP:
			case JN:
			case JZ:
			case NOP:
			case HLT:
			case STA:
			default:
				_state = State::CS_HLT;  // should not reach
				break;
			}
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
				switch (_instruction)
				{
				case NOP:
					end_instruction();
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
					_state = CS_LOAD_OP_DATA;
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
		SC_METHOD(process);
		sensitive << _state;

		SC_METHOD(next_state);
		sensitive_pos << clock;
	}

	unsigned int cycles() { return _cycles; }
	unsigned int instruction_count() { return _inst_count; }

private:
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

	ISA _instruction;
	bool _n_flag;
	bool _z_flag;

	unsigned int _cycles;
	unsigned int _inst_count;
};

#endif /* CONTROL_H_ */