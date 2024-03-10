#include <vector>
#include <iostream>
#include <fstream>
#include <bitset>

using namespace std;

class Assembler{
private:
    // Maps assembly language symbols to their respective binary indices.
    vector<string> symbolTable;
    // Holds each line from a source file as a string for processing.
    vector<string> fileContents;
    // Tracks the number of variables encountered for indexing purposes.
    int varCount;

    // Translates a single line of assembly code into binary format.
    bitset<32> convertLine(string line);

    // Retrieves the binary representation of an operand's address.
    string getOperandAddress(string line, int pos);

public:
    // Constructor initializes the assembler, setting up any necessary tables or counters.
    Assembler();

    // Converts an integer to its 32-bit binary representation, using big-endian format.
    bitset<32> decimalToBinary(int number);

    // Converts a 32-bit binary number to its decimal equivalent.
    int binaryToDecimal(bitset<32> binary);

    // Loads assembly instructions from a specified text file into memory.
    void read(string fileName);

    // Outputs the assembled binary code to a specified text file.
    void write(string fileName);
};
