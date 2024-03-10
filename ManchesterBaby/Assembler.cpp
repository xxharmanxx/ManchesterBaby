#include "Assembler.h"
#include <string>

using namespace std;

Assembler::Assembler(){
  symbolTable = vector<string>();
  varCount = 0;
}

bitset<32> Assembler::decimalToBinary(int number){
  bitset<32> binary;
  binary.reset();
  int rem;
  int num = number;

  for(int i=31; i>=0; i--){
    if(num == 1){
      binary[i] = 1;
      return binary;
    }
    else if(num == 0){
      return binary;
    }
    rem = num%2;
    binary[i] = rem;
    num = num/2;
  }
}

bitset<32> Assembler::convertLine(string line){
  string instructions[8] = {"JMP", "JRP", "LDN", "STO", "SUB", "CMP", "STP", "VAR"};
  int pos = 0;
  int counter = 0;
  int iValue = 0;
  int writePos = 31;
  string tempNum = "0";
  bitset<32> codeReturn;
    for(counter=0; counter<8; counter++){
      cout << counter << endl;
      pos = line.find(instructions[counter], 0); //line.find() returns -1 if an instruction is not found
      cout << pos << endl;
      if(pos != -1){
        iValue = counter;
        counter = 8;
      }
    }
    if(iValue == 0){
      codeReturn[writePos] = 0;
      writePos--;
      codeReturn[writePos] = 0;
      writePos--;
      codeReturn[writePos] = 0;
      writePos--;
      pos = 4;
      string binaryAddress = getOperandAddress(line, pos);
      for(int i=0; i<5; i++, writePos--){
        codeReturn[writePos] = binaryAddress[i];
      }
    }
    else if(iValue == 1){
      codeReturn[writePos] = 1;
      writePos--;
      codeReturn[writePos] = 0;
      writePos--;
      codeReturn[writePos] = 0;
      writePos--;
      pos = 4;
      string binaryAddress = getOperandAddress(line, pos);
      for(int i=0; i<5; i++, writePos--){
        codeReturn[writePos] = binaryAddress[i];
      }
    }
    else if(iValue == 2){
      codeReturn[writePos] = 0;
      writePos--;
      codeReturn[writePos] = 1;
      writePos--;
      codeReturn[writePos] = 0;
      writePos--;
      pos = 4;
      string binaryAddress = getOperandAddress(line, pos);
      for(int i=0; i<5; i++, writePos--){
        codeReturn[writePos] = binaryAddress[i];
      }
    }
    else if(iValue == 3){
      codeReturn[writePos] = 1;
      writePos--;
      codeReturn[writePos] = 1;
      writePos--;
      codeReturn[writePos] = 0;
      writePos--;
      pos = 4;
      string binaryAddress = getOperandAddress(line, pos);
      for(int i=0; i<5; i++, writePos--){
        codeReturn[writePos] = binaryAddress[i];
      }
    }
    else if(iValue == 4){
      codeReturn[writePos] = 0;
      writePos--;
      codeReturn[writePos] = 0;
      writePos--;
      codeReturn[writePos] = 1;
      writePos--;
      pos = 4;
      string binaryAddress = getOperandAddress(line, pos);
      for(int i=0; i<5; i++, writePos--){
        codeReturn[writePos] = binaryAddress[i];
      }
    }
    else if(iValue == 5){
      codeReturn[writePos] = 0;
      writePos--;
      codeReturn[writePos] = 1;
      writePos--;
      codeReturn[writePos] = 1;
      writePos--;
      pos = 4;
      string binaryAddress = getOperandAddress(line, pos);
      for(int i=0; i<5; i++, writePos--){
        codeReturn[writePos] = binaryAddress[i];
      }
    }
    else if(iValue == 6){
      codeReturn[writePos] = 1;
      writePos--;
      codeReturn[writePos] = 1;
      writePos--;
      codeReturn[writePos] = 1;
      writePos--;
    }
    else if(iValue == 7){
      string tempString;
      tempNum = "0";
      if(pos == 0){
        tempString = tempString + ":" + to_string(varCount) + ":";
        symbolTable.push_back(tempString);
	varCount++;
      }
      else{
        int tempPos = pos;
        pos = 0;
        while(line[pos] != ':'){
          tempString = tempString + line[pos];
	  pos++;
        }
        tempString = tempString + ":" + to_string(varCount) + ":";
        pos = tempPos + 4;
        while(line[pos] != ' '){
          tempNum = tempNum + line[pos];
          pos++;
         }
	tempString = tempString + tempNum;
        symbolTable.push_back(tempString);
	varCount++;
      }
    }
  bitset<32> binNum = decimalToBinary(stoi(tempNum));
  for(int i=31; i>=0; i--){
    codeReturn[i] = binNum[i];
  }
  return codeReturn;
}

void Assembler::read(string fileName){
  string currentLine;
  ifstream fileStream;
  fileStream.open(fileName);


  while(getline(fileStream,currentLine)){
    fileContents.push_back(currentLine);
  }
}

  string Assembler::getOperandAddress(string line, int pos){
    string opName;
    string tempString;
    while(line[pos] != ' ' || line[pos] != '\n'){
      opName = opName + line[pos];
      pos++;
    }
    for(int i=0; i<symbolTable.size(); i++){
      int z = 0;
      while(symbolTable[i][z] != ':'){
        tempString = tempString + symbolTable[i][z];
        z++;
      }
      if(opName.compare(tempString)){
        z++;
        string numString;
        while(symbolTable[i][z] != ':'){
          numString = numString + symbolTable[i][z];
          z++;
        }
        int addressInt = stoi(numString);
        bitset<32> addressBinary = decimalToBinary(addressInt);
	string tempString = addressBinary.to_string();
        return tempString;
      }
      else{
        z = 1;
        string numString;
        while(symbolTable[i][z] != ':'){
          numString = numString + symbolTable[i][z];
          z++;
        }
        int addressInt = stoi(numString);
        bitset<32> addressBinary = decimalToBinary(addressInt);
	string tempString = addressBinary.to_string();
        return tempString;
      }
    }
  }

  void Assembler::write(string fileName){
    ofstream fileStream;
    fileStream.open(fileName);

    for(int i=0; i<fileContents.size(); i++){
      fileStream << convertLine(fileContents[i]) << endl;
    }
    fileStream.close();
  }

