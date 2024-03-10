import java.io.*;
import java.nio.file.*;
import java.util.*;
import java.util.regex.*;

public class Assembler {
    private Map<String, Integer> symbolTable = new HashMap<>();
    private List<String> binaryCode = new ArrayList<>();
    private int address = 0;
    private boolean hasErrors = false;

    public void assemble(String inputPath, String outputPath) throws IOException {
        System.out.println("Starting assembly process...");

        List<String> lines = Files.readAllLines(Paths.get(inputPath));
        System.out.println("First pass: Analyzing labels and building symbol table.");
        firstPass(lines);

        if (!hasErrors) {
            System.out.println("Second pass: Translating instructions.");
            secondPass(lines);

            if (!hasErrors) {
                writeOutput(outputPath);
                System.out.println("Assembly completed successfully. Output written to " + outputPath);
            } else {
                System.err.println("Assembly failed due to errors in the second pass.");
            }
        } else {
            System.err.println("Assembly failed due to errors in the first pass.");
        }
    }

    private void firstPass(List<String> lines) {
        Pattern labelPattern = Pattern.compile("^(\\w+):");
        for (int i = 0; i < lines.size(); i++) {
            Matcher matcher = labelPattern.matcher(lines.get(i));
            if (matcher.find()) {
                String label = matcher.group(1);
                if (symbolTable.containsKey(label)) {
                    System.err.println("Error on line " + (i + 1) + ": Duplicate label '" + label + "' found.");
                    hasErrors = true;
                } else {
                    symbolTable.put(label, address);
                }
            } else if (!lines.get(i).trim().isEmpty()) {
                address++;
            }
        }
    }

    private void secondPass(List<String> lines) {
        Pattern instrPattern = Pattern.compile("^(\\w+)\\s*(\\w*)");
        for (int i = 0; i < lines.size(); i++) {
            Matcher matcher = instrPattern.matcher(lines.get(i));
            if (matcher.find()) {
                String instruction = matcher.group(1).toUpperCase();
                String operand = matcher.group(2);
                String binaryInstruction = translateInstruction(instruction, operand, i + 1);
                if (binaryInstruction != null) {
                    binaryCode.add(binaryInstruction);
                }
            } else if (!lines.get(i).trim().isEmpty()) {
                System.err.println("Error on line " + (i + 1) + ": Syntax error or unsupported instruction format.");
                hasErrors = true;
            }
        }
    }


    private String translateInstruction(String instruction, String operand, int lineNumber) {
        int opcode;
        int operandAddress = operand.isEmpty() ? 0 : symbolTable.getOrDefault(operand, -1);
        if (operandAddress == -1) {
            System.err.println("Error on line " + lineNumber + ": Undefined symbol or label - " + operand);
            hasErrors = true;
            return null;
        }

        switch (instruction.toUpperCase()) {
            case "JMP": opcode = 0; break;
            case "JRP": opcode = 1; break;
            case "LDN": opcode = 2; break;
            case "STO": opcode = 3; break;
            case "SUB": opcode = 4; break;
            case "CMP": opcode = 5; break;
            case "STP": opcode = 6; break;
            case "ADD": opcode = 7; break;
            case "NOP": opcode = 8; break;
            case "MUL": opcode = 9; break;
            case "DIV": opcode = 10; break;
            case "AND": opcode = 11; break;
            case "OR": opcode = 12; break;
            case "XOR": opcode = 13; break;
            case "BRZ": opcode = 14; break;
            default:
                System.err.println("Error on line " + lineNumber + ": Unsupported instruction '" + instruction + "'.");
                hasErrors = true;
                return null;
        }

        String binaryOpcode = String.format("%5s", Integer.toBinaryString(opcode)).replace(' ', '0');
        String binaryOperandAddress = String.format("%27s", Integer.toBinaryString(operandAddress)).replace(' ', '0');
        return binaryOpcode + binaryOperandAddress;
    }

    private void writeOutput(String outputPath) throws IOException {
        Files.write(Paths.get(outputPath), binaryCode);
    }

    public static void main(String[] args) {
        if (args.length != 2) {
            System.out.println("Usage: java Assembler <input assembly file> <output binary file>");
            System.exit(1);
        }
        Assembler assembler = new Assembler();
        try {
            assembler.assemble(args[0], args[1]);
            System.out.println("Assembly completed successfully.");
        } catch (IOException e) {
            System.out.println("Failed to assemble program.");
            e.printStackTrace();
        }
    }
}
