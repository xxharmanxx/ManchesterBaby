#ifndef _BABY
#define _BABY
#include <bitset>
#include <vector>

using namespace std;

// Template function to reverse the order of bits in a bitset.
template <size_t T>
bitset<T> reverse(bitset<T> initialSet);

class Baby {
private:
    // Retrieves the operand's address from an instruction's bitset.
    bitset<5> getOperandAddress(bitset<32> instruction);
    // Fetches the instruction or data from the store based on a given address.
    bitset<32> fetchFromStore(bitset<5> address);
    // Extracts the operation code from an instruction's bitset.
    bitset<3> getOperator(bitset<32> instruction);

    // Performs binary addition on two 32-bit bitsets.
    bitset<32> binaryAdder(bitset<32> arg1, bitset<32> arg2);
    // Converts an opcode bitset into its corresponding instruction index.
    int convertOpcodeToIndex(bitset<3> opcode);
    // Converts a binary address to its decimal index equivalent.
    int convertAddressToIndex(bitset<5> address);
    // Converts a 32-bit binary number to its decimal equivalent.
    int convertBinaryToDecimal(bitset<32> convertThis);

    // Type definition for function pointers to Baby's instruction methods.
    typedef void (Baby::*instruction_ptr)(bitset<5>);

    // Instruction Methods
    void JMP(bitset<5> operand); // Jump to a specific address in the store.
    void JRP(bitset<5> operand); // Jump relative to the current address.
    void LDN(bitset<5> operand); // Load a negative value from the store into the accumulator.
    void STO(bitset<5> operand); // Store the accumulator's value into a specified address.
    void SUB(bitset<5> operand); // Subtract a store value from the accumulator.
    void CMP(bitset<5> operand); // Compare accumulator's value and set test flop if negative.
    void STOP(bitset<5> operand); // Halt the simulation.

    // Instruction set mapping opcodes to their corresponding functions.
    instruction_ptr symbolTable[8] = {&Baby::JMP, &Baby::JRP, &Baby::LDN, &Baby::STO, &Baby::SUB, &Baby::SUB, &Baby::CMP, &Baby::STOP};

public:
    Baby(); // Default constructor initializes the machine state.
    Baby(vector<bitset<32>> registers); // Constructor to initialize the store with predefined values.

    // Machine State Registers
    bitset<32> CI; // Control Instruction register pointing to the next instruction to execute.
    bitset<32> PI; // Present Instruction register holding the current instruction.
    bitset<32> accumulator; // Accumulator for holding arithmetic and logic operation results.
    vector<bitset<32>> store; // Memory store, consisting of 32 32-bit registers.

    // Flags
    bool testFlop; // Flag used for conditional operations.
    bool stopped; // Indicates whether the machine has been halted.
    bool stepExecution; // Controls step-by-step execution for debugging.

    // Functions for fetching and executing instructions
    void increment(); // Increments the CI to point to the next instruction.
    void fetch(); // Fetches the next instruction into the PI.
    void decode(); // Decodes the current instruction in the PI.
    void execute(bitset<3> opcode, bitset<5> operandAddress); // Executes the decoded instruction.

    // Display Functions
    bitset<32> getCI(); // Returns the current CI value.
    bitset<32> getPI(); // Returns the current PI value.
    void printRegisters(); // Prints the current state of the machine's registers.
};

#endif
