import java.util.*;
import java.io.*;

public class Assembler {
  private List<String> symbolTable;
  private int varCount;
  private List<String> fileContents;

  public Assembler() {
    symbolTable = new ArrayList<String>();
    varCount = 0;
    fileContents = new ArrayList<String>();
  }

  public BitSet decimalToBinary(int number) {
    BitSet binary = new BitSet(32);
    binary.clear();
    int rem;
    int num = number;
    for (int i = 31; i >= 0; i--) {
      if (num == 1) {
        binary.set(i);
        return binary;
      } else if (num == 0) {
        return binary;
      }
      rem = num % 2;
      binary.set(i, rem == 1);
      num = num / 2;
    }
    return binary;
  }

  public BitSet convertLine(String line) {
    String[] instructions = {"JMP", "JRP", "LDN", "STO", "SUB", "CMP", "STP", "VAR"};
    int pos = 0;
    int counter = 0;
    int iValue = 0;
    int writePos = 31;
    String tempNum = "0";
    BitSet codeReturn = new BitSet(32);
    for (counter = 0; counter < 8; counter++) {
      pos = line.indexOf(instructions[counter], 0);
      if (pos != -1) {
        iValue = counter;
        counter = 8;
      }
    }
    if (iValue == 0) {
      codeReturn.set(writePos--, false);
      codeReturn.set(writePos--, false);
      codeReturn.set(writePos--, false);
      pos = 4;
      String binaryAddress = getOperandAddress(line, pos);
      for (int i = 0; i < 5; i++, writePos--) {
        codeReturn.set(writePos, binaryAddress.charAt(i) == '1');
      }
    } else if (iValue == 1) {
      codeReturn.set(writePos--, true);
      codeReturn.set(writePos--, false);
      codeReturn.set(writePos--, false);
      pos = 4;
      String binaryAddress = getOperandAddress(line, pos);
      for (int i = 0; i < 5; i++, writePos--) {
        codeReturn.set(writePos, binaryAddress.charAt(i) == '1');
      }
    } else if (iValue == 2) {
      codeReturn.set(writePos--, false);
      codeReturn.set(writePos--, true);
      codeReturn.set(writePos--, false);
      pos = 4;
      String binaryAddress = getOperandAddress(line, pos);
      for (int i = 0; i < 5; i++, writePos--) {
        codeReturn.set(writePos, binaryAddress.charAt(i) == '1');
      }
    } else if (iValue == 3) {
      codeReturn.set(writePos--, true);
      codeReturn.set(writePos--, true);
      codeReturn.set(writePos--, false);
      pos = 4;
      String binaryAddress = getOperandAddress(line, pos);
      for (int i = 0; i < 5; i++, writePos--) {
        codeReturn.set(writePos, binaryAddress.charAt(i) == '1');
      }
    } else if (iValue == 4) {
      codeReturn.set(writePos--, false);
      codeReturn.set(writePos--, false);
      codeReturn.set(writePos--, true);
      pos = 4;
      String binaryAddress = getOperandAddress(line, pos);
      for (int i = 0; i < 5; i++, writePos--) {
        codeReturn.set(writePos, binaryAddress.charAt(i) == '1');
      }
    } else if (iValue == 5) {
      codeReturn.set(writePos--, false);
      codeReturn.set(writePos--, true);
      codeReturn.set(writePos--, true);
      pos = 4;
      String binaryAddress = getOperandAddress(line, pos);
      for (int i = 0; i < 5; i++, writePos--) {
        codeReturn.set(writePos, binaryAddress.charAt(i) == '1');
      }
    } else if (iValue == 6) {
      codeReturn.set(writePos--, true);
      codeReturn.set(writePos--, true);
      codeReturn.set(writePos--, true);
    } else if (iValue == 7) {
      String tempString;
      tempNum = "0";
      if (pos == 0) {
        tempString = ":" + varCount + ":";
        symbolTable.add(tempString);
        varCount++;
      } else {
        int tempPos = pos;
        pos = 0;
        while (line.charAt(pos) != ':') {
          tempString = tempString + line.charAt(pos);
          pos++;
        }
        tempString = tempString + ":" + varCount + ":";
        pos = tempPos + 4;
        while (line.charAt(pos) != ' ') {
          tempNum = tempNum + line.charAt(pos);
          pos++;
        }
        tempString = tempString + tempNum;
        symbolTable.add(tempString);
        varCount++;
      }
    }
    BitSet binNum = decimalToBinary(Integer.parseInt(tempNum));
    for (int i = 31; i >= 0; i--) {
      codeReturn.set(i, binNum.get(i));
    }
    return codeReturn;
  }

  public void read(String fileName) {
    String currentLine;
    try {
      BufferedReader fileStream = new BufferedReader(new FileReader(fileName));
      while ((currentLine = fileStream.readLine()) != null) {
        fileContents.add(currentLine);
      }
      fileStream.close();
    } catch (IOException e) {
      e.printStackTrace();
    }
  }

  public String getOperandAddress(String line, int pos) {
    String opName;
    String tempString = "";
    while (line.charAt(pos) != ' ' && line.charAt(pos) != '\n') {
      opName = opName + line.charAt(pos);
      pos++;
    }
    for (int i = 0; i < symbolTable.size(); i++) {
      int z = 0;
      while (symbolTable.get(i).charAt(z) != ':') {
        tempString = tempString + symbolTable.get(i).charAt(z);
        z++;
      }
      if (opName.equals(tempString)) {
        z++;
        String numString = "";
        while (symbolTable.get(i).charAt(z) != ':') {
          numString = numString + symbolTable.get(i).charAt(z);
          z++;
        }
        int addressInt = Integer.parseInt(numString);
        BitSet addressBinary = decimalToBinary(addressInt);
        String tempString = addressBinary.toString();
        return tempString;
      } else {
        z = 1;
        String numString = "";
        while (symbolTable.get(i).charAt(z) != ':') {
          numString = numString + symbolTable.get(i).charAt(z);
          z++;
        }
        int addressInt = Integer.parseInt(numString);
        BitSet addressBinary = decimalToBinary(addressInt);
        String tempString = addressBinary.toString();
        return tempString;
      }
    }
    return "";
  }

  public void write(String fileName) {
    try {
      BufferedWriter fileStream = new BufferedWriter(new FileWriter(fileName));
      for (int i = 0; i < fileContents.size(); i++) {
        BitSet convertedLine = convertLine(fileContents.get(i));
        fileStream.write(convertedLine.toString() + "\n");
      }
      fileStream.close();
    } catch (IOException e) {
      e.printStackTrace();
    }
  }
}


