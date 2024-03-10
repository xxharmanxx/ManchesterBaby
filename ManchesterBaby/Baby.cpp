#include "Baby.h"
#include <iostream>
#include <cmath>

using namespace std;

// Reverse the order of bits in a bitset, useful for endianness adjustments or visual representation.
template <size_t T>
bitset<T> reverse(bitset<T> initialSet){
    bitset<T> reversed = bitset<T>();
    for(int i = 0; i < T; i++){
        reversed[i] = initialSet[(T-1)-i];
    }
    return reversed;
}
// Convert a binary bitset to its decimal representation, accounting for the sign bit.
template <size_t T>
int binaryToDecimal(bitset<T> binary){
    bool isNegative = binary[T-1];
    int decimal = 0;
    for(int i = 0; i < T-1; i++){
        if (binary[i])
            decimal += pow(2, i);
    }
    if(isNegative){
        decimal *= -1;
    }
    return decimal;
}

// Constructor for initializing a Baby instance with default settings.
Baby::Baby(){
    bitset<32> initialRegister;
    store.resize(32, initialRegister);
    bitset<5> op;
    stepExecution = false;
    stopped = false;
    testFlop = false;

    int x = 0;
    while(x<=7){
        ((*this).*symbolTable[x])(op);
        x++;
    }
}

Baby::Baby(vector<bitset<32>> registers){
    store = registers;

    CI = bitset<32>();
    stopped = false;
    testFlop = false;
    accumulator = bitset<32>();
}

int Baby::convertOpcodeToIndex(bitset<3> opcode){
    bitset<3> reversed = bitset<3>();
    for(int i = 2; i >= 0; i--){
        reversed[i] = opcode[2-i];
    }
    return reversed.to_ulong();
}

int Baby::convertBinaryToDecimal(bitset<32> convertThis){
    int x = 0;
    for(int i = 0; i <= 30; i++ ){
        convertThis[i] ? x += pow(2, i) : false;
    }
    return x;
}

bitset<32> Baby::binaryAdder(bitset<32> arg1, bitset<32> arg2){
    bitset<32> sum = bitset<32>();
    bool carry = false;
    for(int i = 0; i < 32; i++){
        sum[i] = (arg1[i] ^ arg2[i]) ^ carry;
        carry = ((arg1[i] & arg2[i]) | (arg1[i] & carry)) | (arg2[i] & carry);
    }
    return sum;
}

// The following functions implement the Manchester Baby's instructions.
// Each function modifies the machine state according to its respective instruction.

void Baby::JMP(bitset<5> operand){
    cout << "JMP" << endl;
    CI = fetchFromStore(operand);

    cout << CI << endl;
}

void Baby::JRP(bitset<5> operand){
    cout << "JRP" << endl;

    bitset<32> increment = fetchFromStore(operand);
    CI = binaryAdder(CI, increment);
}

void Baby::LDN(bitset<5> operand){
    bitset<32> storeLine = fetchFromStore(operand);
    storeLine[31] = !storeLine[31];
    accumulator = storeLine;
    cout << "LDN" << endl;
    cout << "StoreLine: " << reverse(storeLine) << endl;
    cout << binaryToDecimal(storeLine) << endl;
}

void Baby::STO(bitset<5> operand){
    cout << "STO" << endl;
    int storeIndex = operand.to_ulong();
    store[storeIndex] = accumulator;
}

void Baby::SUB(bitset<5> operand){
    cout << "SUB" << endl;
    bitset<32> storeLine = fetchFromStore(operand);
    bitset<32> product  = bitset<32>();
    bool borrow = false;
    for(int i = 0; i <32; i++){
        product[i] = accumulator[i] ^ storeLine[i] ^ borrow;
        borrow = (~accumulator[i] & (borrow | storeLine[i])) | storeLine[i] & borrow;
    }
    cout << "Accumulator: " << reverse(accumulator) << endl;
    accumulator = product;
    cout << "StoreLine: " << reverse(storeLine) << endl;
    cout << "Product: " << reverse(product) << endl;
}

void Baby::CMP(bitset<5> operand){
    cout << "CMP" << endl;
    if(accumulator[31]){
        testFlop = true;
    }
}

void Baby::STOP(bitset<5> operand){
    cout << "STOP" << endl;
    stopped = true;
}

bitset<32> Baby::fetchFromStore(bitset<5> address){
    bitset<32> registerContents = bitset<32>();
    unsigned int addressIndex = address.to_ulong();
    registerContents = store[addressIndex];
    return registerContents;
}

void Baby::fetch(){
    bitset<5> PIAddress = getOperandAddress(CI);
    PI = fetchFromStore(PIAddress);
    decode();
}

void Baby::increment(){
    cout << ">> increment" << endl;
    bitset<32> increment = bitset<32>();
    if(testFlop){
        increment[1] = true;
        testFlop = false;
    }
    else{
        increment[0] = 1;
    }
    CI = binaryAdder(CI, increment);
    cout << "<< increment increment is: "<< increment <<  "\n CI is: " << CI << endl;
    fetch();
}

void Baby::decode(){
    bitset<3> opcode = getOperator(PI);
    bitset<5> operandAddress = getOperandAddress(PI);
    execute(opcode, operandAddress);
}

void Baby::execute(bitset<3> opcode, bitset<5> operandAddress){
    unsigned int opcodeIndex = opcode.to_ulong();
    ((*this).*symbolTable[opcodeIndex])(operandAddress);
}

bitset<5> Baby::getOperandAddress(bitset<32> instruction){
    bitset<5> operand = bitset<5>();
    for(int i = 0; i <= 4; i++){
        operand[i] = instruction[i];
    }
    return operand;
}

bitset<3> Baby::getOperator(bitset<32> instruction){
    bitset<3> opcode = bitset<3>();
    for(int i = 13; i <= 15; i++){
        opcode[i-13] = instruction[i];
    }
    return opcode;
}
