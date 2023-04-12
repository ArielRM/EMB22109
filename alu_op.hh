#ifndef ALU_OP_HH_
#define ALU_OP_HH_

#include <systemc>

class ALUOperation
{
public:
    enum Operation : unsigned
    {
        NOP = 0,
        ADD = 1,
        AND = 2,
        OR = 3,
        NOT = 4,
        LDA = 5,
        UKN,
    };

    ALUOperation(const unsigned int i)
    {
        if (i <= 5)
        {
            _value = static_cast<Operation>(i);
        }
        else
        {
            _value = Operation::UKN;
        }
    }

    // para facilitar sinal.read() -> enum
    ALUOperation(const sc_dt::sc_uint<3> i) : ALUOperation(static_cast<const unsigned>(i)){};
    constexpr ALUOperation(Operation op) : _value(op) {}

    constexpr operator Operation() const { return _value; } // Permite que a classe seja usada como enum
    
    // para facilitar enum -> sinal.write()
    operator const sc_dt::sc_uint<3>() const { return _value; }

    explicit operator bool() const = delete; // Não permite que seja usada com if(op) {}
    constexpr bool operator==(ALUOperation op) const { return _value == op._value; }
    constexpr bool operator!=(ALUOperation op) const { return _value != op._value; }

    // Conversão rápida para `const char *`
    constexpr operator const char *() const
    {
        switch (_value)
        {
        case NOP:
            return "NOP";
        case ADD:
            return "ADD";
        case AND:
            return "AND";
        case OR:
            return "OR ";
        case NOT:
            return "NOT";
        case LDA:
            return "LDA";
        default:
            return "UNK";
        }
    }

private:
    Operation _value;
};

#endif /* ALU_OP_HH_ */