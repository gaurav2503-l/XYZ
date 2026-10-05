#include <cstdint>
#include <array>
#include <iostream>
#include <iomanip>

using u32 = uint32_t;

enum class Opcode : uint8_t {
    ADD  = 0b00001,
    SUB  = 0b00010,
    MUL  = 0b00011,
    DIV  = 0b00100,
    MOD  = 0b00101,
    CMP  = 0b00110,
    AND_ = 0b00111,
    OR   = 0b01000,
    NOT  = 0b01001,
    XOR  = 0b01010,
    MOV  = 0b01011,
    MOVU = 0b01100,
    MOVH = 0b01101,
    LSL  = 0b01110,
    LSR  = 0b01111,
    ASR  = 0b10000,
    NOP  = 0b10001,
    LD   = 0b10010,
    ST   = 0b10011,
    BEQ  = 0b10100,
    BGT  = 0b10101,
    BSM  = 0b10110,
    B    = 0b10111,
    CALL = 0b11000,
    RET  = 0b11001
};

enum class ALU {
    NONE, ADD, SUB, MUL, DIV, MOD,
    AND_, OR, XOR, NOT
};

enum class SHIFT {
    NONE, LSL, LSR, ASR
};

enum class PC_SRC {
    NONE, PC, BRANCH, R1
};

enum class BRANCH {
    NONE, BEQ, BGT, BSM, B
};

enum class MUX {
    NONE, REG, IMM, PC, SP, FP
};

struct MicroInstruction {
    bool IR_LOAD = false;
    bool PC_INC = false;
    bool PC_LOAD = false;

    bool REG_READ = false;
    bool REG_WRITE = false;

    ALU alu = ALU::NONE;
    SHIFT shift = SHIFT::NONE;

    bool MEM_READ = false;
    bool MEM_WRITE = false;

    MUX OP_A_SEL = MUX::NONE;
    MUX OP_B_SEL = MUX::NONE;

    bool CMP_UPDATE = false;

    BRANCH branch = BRANCH::NONE;

    bool EXC_SAVE = false;
    bool EXC_VECTOR = false;

    bool NEXT = false;
    bool DISPATCH = false;
    bool COND = false;
    bool END = false;
};

const char* aluName(ALU x) {
    switch (x) {
        case ALU::ADD:  return "ADD";
        case ALU::SUB:  return "SUB";
        case ALU::MUL:  return "MUL";
        case ALU::DIV:  return "DIV";
        case ALU::MOD:  return "MOD";
        case ALU::AND_: return "AND";
        case ALU::OR:   return "OR";
        case ALU::XOR:  return "XOR";
        case ALU::NOT:  return "NOT";
        default:        return "-";
    }
}

const char* shiftName(SHIFT x) {
    switch (x) {
        case SHIFT::LSL: return "LSL";
        case SHIFT::LSR: return "LSR";
        case SHIFT::ASR: return "ASR";
        default:         return "-";
    }
}

const char* branchName(BRANCH x) {
    switch (x) {
        case BRANCH::BEQ: return "BEQ";
        case BRANCH::BGT: return "BGT";
        case BRANCH::BSM: return "BSM";
        case BRANCH::B:   return "B";
        default:          return "-";
    }
}

const char* muxName(MUX x) {
    switch (x) {
        case MUX::REG: return "REG";
        case MUX::IMM: return "IMM";
        case MUX::PC:  return "PC";
        case MUX::SP:  return "SP";
        case MUX::FP:  return "FP";
        default:       return "-";
    }
}

const char* opcodeName(Opcode op) {
    switch (op) {
        case Opcode::ADD:  return "ADD";
        case Opcode::SUB:  return "SUB";
        case Opcode::MUL:  return "MUL";
        case Opcode::DIV:  return "DIV";
        case Opcode::MOD:  return "MOD";
        case Opcode::CMP:  return "CMP";
        case Opcode::AND_: return "AND";
        case Opcode::OR:   return "OR";
        case Opcode::NOT:  return "NOT";
        case Opcode::XOR:  return "XOR";
        case Opcode::MOV:  return "MOV";
        case Opcode::MOVU: return "MOVU";
        case Opcode::MOVH: return "MOVH";
        case Opcode::LSL:  return "LSL";
        case Opcode::LSR:  return "LSR";
        case Opcode::ASR:  return "ASR";
        case Opcode::NOP:  return "NOP";
        case Opcode::LD:   return "LD";
        case Opcode::ST:   return "ST";
        case Opcode::BEQ:  return "BEQ";
        case Opcode::BGT:  return "BGT";
        case Opcode::BSM:  return "BSM";
        case Opcode::B:    return "B";
        case Opcode::CALL: return "CALL";
        case Opcode::RET:  return "RET";
    }
    return "?";
}



MicroInstruction FETCH() {
    MicroInstruction m;

    m.IR_LOAD = true;
    m.PC_INC = true;
    m.MEM_READ = true;

    m.NEXT = true;

    return m;
}

MicroInstruction DECODE() {
    MicroInstruction m;

    m.REG_READ = true;
    m.DISPATCH = true;

    return m;
}



MicroInstruction ADD_EX() {
    MicroInstruction m;

    m.REG_READ = true;
    m.OP_A_SEL = MUX::REG;
    m.OP_B_SEL = MUX::REG;
    m.alu = ALU::ADD;
    m.NEXT = true;

    return m;
}

MicroInstruction SUB_EX() {
    MicroInstruction m;

    m.REG_READ = true;
    m.OP_A_SEL = MUX::REG;
    m.OP_B_SEL = MUX::REG;
    m.alu = ALU::SUB;
    m.NEXT = true;

    return m;
}

MicroInstruction MUL_EX() {
    MicroInstruction m;

    m.REG_READ = true;
    m.OP_A_SEL = MUX::REG;
    m.OP_B_SEL = MUX::REG;
    m.alu = ALU::MUL;
    m.NEXT = true;

    return m;
}

MicroInstruction DIV_EX() {
    MicroInstruction m;

    m.REG_READ = true;
    m.OP_A_SEL = MUX::REG;
    m.OP_B_SEL = MUX::REG;
    m.alu = ALU::DIV;
    m.NEXT = true;

    return m;
}

MicroInstruction MOD_EX() {
    MicroInstruction m;

    m.REG_READ = true;
    m.OP_A_SEL = MUX::REG;
    m.OP_B_SEL = MUX::REG;
    m.alu = ALU::MOD;
    m.NEXT = true;

    return m;
}


MicroInstruction CMP_EX() {
    MicroInstruction m;

    m.REG_READ = true;
    m.OP_A_SEL = MUX::REG;
    m.OP_B_SEL = MUX::REG;
    m.alu = ALU::SUB;
    m.CMP_UPDATE = true;
    m.NEXT = true;

    return m;
}



MicroInstruction AND_EX() {
    MicroInstruction m;

    m.REG_READ = true;
    m.OP_A_SEL = MUX::REG;
    m.OP_B_SEL = MUX::REG;
    m.alu = ALU::AND_;
    m.NEXT = true;

    return m;
}

MicroInstruction OR_EX() {
    MicroInstruction m;

    m.REG_READ = true;
    m.OP_A_SEL = MUX::REG;
    m.OP_B_SEL = MUX::REG;
    m.alu = ALU::OR;
    m.NEXT = true;

    return m;
}

MicroInstruction NOT_EX() {
    MicroInstruction m;

    m.REG_READ = true;
    m.OP_A_SEL = MUX::REG;
    m.alu = ALU::NOT;
    m.NEXT = true;

    return m;
}

MicroInstruction XOR_EX() {
    MicroInstruction m;

    m.REG_READ = true;
    m.OP_A_SEL = MUX::REG;
    m.OP_B_SEL = MUX::REG;
    m.alu = ALU::XOR;
    m.NEXT = true;

    return m;
}



MicroInstruction MOV_EX() {
    MicroInstruction m;

    m.REG_READ = true;
    m.OP_A_SEL = MUX::REG;
    m.NEXT = true;

    return m;
}

MicroInstruction MOVU_EX() {
    MicroInstruction m;

    m.OP_A_SEL = MUX::IMM;
    m.NEXT = true;

    return m;
}

MicroInstruction MOVH_EX() {
    MicroInstruction m;

    m.OP_A_SEL = MUX::IMM;
    m.NEXT = true;

    return m;
}


MicroInstruction LSL_EX() {
    MicroInstruction m;

    m.REG_READ = true;
    m.OP_A_SEL = MUX::REG;
    m.OP_B_SEL = MUX::REG;
    m.shift = SHIFT::LSL;
    m.NEXT = true;

    return m;
}

MicroInstruction LSR_EX() {
    MicroInstruction m;

    m.REG_READ = true;
    m.OP_A_SEL = MUX::REG;
    m.OP_B_SEL = MUX::REG;
    m.shift = SHIFT::LSR;
    m.NEXT = true;

    return m;
}

MicroInstruction ASR_EX() {
    MicroInstruction m;

    m.REG_READ = true;
    m.OP_A_SEL = MUX::REG;
    m.OP_B_SEL = MUX::REG;
    m.shift = SHIFT::ASR;
    m.NEXT = true;

    return m;
}



MicroInstruction NOP_EX() {
    MicroInstruction m;

    m.NEXT = true;

    return m;
}


MicroInstruction LD_EX() {
    MicroInstruction m;

    m.REG_READ = true;
    m.OP_A_SEL = MUX::REG;
    m.OP_B_SEL = MUX::IMM;
    m.alu = ALU::ADD;
    m.MEM_READ = true;
    m.NEXT = true;

    return m;
}



MicroInstruction ST_EX() {
    MicroInstruction m;

    m.REG_READ = true;
    m.OP_A_SEL = MUX::REG;
    m.OP_B_SEL = MUX::IMM;
    m.alu = ALU::ADD;
    m.MEM_WRITE = true;
    m.NEXT = true;

    return m;
}


MicroInstruction BEQ_EX() {
    MicroInstruction m;

    m.OP_A_SEL = MUX::PC;
    m.OP_B_SEL = MUX::IMM;
    m.alu = ALU::ADD;
    m.PC_LOAD = true;
    m.branch = BRANCH::BEQ;
    m.COND = true;
    m.NEXT = true;

    return m;
}

MicroInstruction BGT_EX() {
    MicroInstruction m;

    m.OP_A_SEL = MUX::PC;
    m.OP_B_SEL = MUX::IMM;
    m.alu = ALU::ADD;
    m.PC_LOAD = true;
    m.branch = BRANCH::BGT;
    m.COND = true;
    m.NEXT = true;

    return m;
}

MicroInstruction BSM_EX() {
    MicroInstruction m;

    m.OP_A_SEL = MUX::PC;
    m.OP_B_SEL = MUX::IMM;
    m.alu = ALU::ADD;
    m.PC_LOAD = true;
    m.branch = BRANCH::BSM;
    m.COND = true;
    m.NEXT = true;

    return m;
}

MicroInstruction B_EX() {
    MicroInstruction m;

    m.OP_A_SEL = MUX::PC;
    m.OP_B_SEL = MUX::IMM;
    m.alu = ALU::ADD;
    m.PC_LOAD = true;
    m.branch = BRANCH::B;
    m.NEXT = true;

    return m;
}


MicroInstruction CALL_EX() {
    MicroInstruction m;

    m.OP_A_SEL = MUX::PC;
    m.OP_B_SEL = MUX::IMM;
    m.alu = ALU::ADD;
    m.PC_LOAD = true;
    m.NEXT = true;

    return m;
}


MicroInstruction RET_EX() {
    MicroInstruction m;

    m.PC_LOAD = true;
    m.OP_A_SEL = MUX::REG;
    m.NEXT = true;

    return m;
}



MicroInstruction WRITEBACK(Opcode op) {
    MicroInstruction m;

    switch (op) {

        case Opcode::ADD:
        case Opcode::SUB:
        case Opcode::MUL:
        case Opcode::DIV:
        case Opcode::MOD:
        case Opcode::AND_:
        case Opcode::OR:
        case Opcode::NOT:
        case Opcode::XOR:
        case Opcode::MOV:
        case Opcode::MOVU:
        case Opcode::MOVH:
        case Opcode::LSL:
        case Opcode::LSR:
        case Opcode::ASR:
        case Opcode::LD:
            m.REG_WRITE = true;
            break;

        default:
            break;
    }

    m.END = true;

    return m;
}



MicroInstruction executeMicrocode(Opcode op) {

    switch (op) {

        case Opcode::ADD:  return ADD_EX();
        case Opcode::SUB:  return SUB_EX();
        case Opcode::MUL:  return MUL_EX();
        case Opcode::DIV:  return DIV_EX();
        case Opcode::MOD:  return MOD_EX();

        case Opcode::CMP:  return CMP_EX();

        case Opcode::AND_: return AND_EX();
        case Opcode::OR:   return OR_EX();
        case Opcode::NOT:  return NOT_EX();
        case Opcode::XOR:  return XOR_EX();

        case Opcode::MOV:  return MOV_EX();
        case Opcode::MOVU: return MOVU_EX();
        case Opcode::MOVH: return MOVH_EX();

        case Opcode::LSL:  return LSL_EX();
        case Opcode::LSR:  return LSR_EX();
        case Opcode::ASR:  return ASR_EX();

        case Opcode::NOP:  return NOP_EX();

        case Opcode::LD:   return LD_EX();
        case Opcode::ST:   return ST_EX();

        case Opcode::BEQ:  return BEQ_EX();
        case Opcode::BGT:  return BGT_EX();
        case Opcode::BSM:  return BSM_EX();
        case Opcode::B:    return B_EX();

        case Opcode::CALL: return CALL_EX();
        case Opcode::RET:  return RET_EX();
    }

    return {};
}


void printMicrocode(Opcode op, int pc) {

    MicroInstruction m;

    if (pc == 0)
        m = FETCH();
    else if (pc == 1)
        m = DECODE();
    else if (pc == 2)
        m = executeMicrocode(op);
    else
        m = WRITEBACK(op);

    std::cout << std::setw(2) << pc << " : ";

    if (m.IR_LOAD)    std::cout << "IR_LOAD ";
    if (m.PC_INC)     std::cout << "PC_INC ";
    if (m.PC_LOAD)    std::cout << "PC_LOAD ";

    if (m.REG_READ)   std::cout << "REG_READ ";
    if (m.REG_WRITE)  std::cout << "REG_WRITE ";

    if (m.alu != ALU::NONE)
        std::cout << "ALU_" << aluName(m.alu) << " ";

    if (m.shift != SHIFT::NONE)
        std::cout << "SHIFT_" << shiftName(m.shift) << " ";

    if (m.MEM_READ)   std::cout << "MEM_READ ";
    if (m.MEM_WRITE)  std::cout << "MEM_WRITE ";

    if (m.OP_A_SEL != MUX::NONE)
        std::cout << "OP_A=" << muxName(m.OP_A_SEL) << " ";

    if (m.OP_B_SEL != MUX::NONE)
        std::cout << "OP_B=" << muxName(m.OP_B_SEL) << " ";

    if (m.CMP_UPDATE)  std::cout << "CMP_UPDATE ";

    if (m.branch != BRANCH::NONE)
        std::cout << "BR_" << branchName(m.branch) << " ";

    if (m.EXC_SAVE)    std::cout << "EXC_SAVE ";
    if (m.EXC_VECTOR)  std::cout << "EXC_VECTOR ";

    if (m.NEXT)        std::cout << "NEXT ";
    if (m.DISPATCH)    std::cout << "DISPATCH ";
    if (m.COND)        std::cout << "COND ";
    if (m.END)         std::cout << "END ";

    std::cout << '\n';
}



int main() {

    for (int op = 1; op <= 25; ++op) {

        Opcode opcode = static_cast<Opcode>(op);

        std::cout << "\n========================================\n";
        std::cout << opcodeName(opcode) << "\n";
        std::cout << "========================================\n";

        printMicrocode(opcode, 0); 
        printMicrocode(opcode, 1); 
        printMicrocode(opcode, 2); 
        printMicrocode(opcode, 3); 
    }

    return 0;
}
