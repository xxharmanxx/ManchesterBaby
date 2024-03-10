import java.io.*;
import java.nio.file.*;
import java.util.*;

public class BabySimulator {
    private String[] memory; // Store
    private int pc = 0; // Program Counter
    private int ci; // Current Instruction
    private int acc = 0; // Accumulator
    private final int memorySize;
    private final int wordLength = 32;

    public BabySimulator(String programPath, int memorySize) throws IOException {
        this.memorySize = memorySize;
        this.memory = new String[this.memorySize];
        loadProgram(programPath);
    }

    private void loadProgram(String path) throws IOException {
        Arrays.fill(memory, "0".repeat(wordLength)); // Initialize memory with zeros
        List<String> lines = Files.readAllLines(Paths.get(path));
        for (int i = 0; i < memorySize && i < lines.size(); i++) {
            memory[i] = lines.get(i);
        }
    }

    public void run() {
        while (true) {
            fetch();
            decodeAndExecute();
            printState();
        }
    }

    private void fetch() {
        ci = Integer.parseInt(memory[pc], 2);
    }

    private void decodeAndExecute() {
        int opcode = (ci >> 13) & 0x7; // Adjust if more bits are needed for opcodes
        int operand = ci & ((1 << 13) - 1); // Adjust accordingly// Assuming the remaining bits are the operand

        switch (opcode) {
            case 0: // JMP
                pc = operand;
                break;
            case 1: // JRP
                pc += operand;
                break;
            case 2: // LDN
                acc = -Integer.parseInt(memory[operand], 2);
                pc++;
                break;
            case 3: // STO
                memory[operand] = Integer.toBinaryString(acc);
                pc++;
                break;
            case 4: // SUB
                acc -= Integer.parseInt(memory[operand], 2);
                pc++;
                break;
            case 5: // CMP
                if (acc < 0) pc++;
                break;
            case 6: // STP
                System.exit(0);
                break;
            case 7: // ADD: Add memory[operand] to accumulator
                acc += Integer.parseInt(memory[operand], 2);
                pc++;
                break;
            case 8: // NOP: No Operation
                pc++;
                break;
            case 9: // MUL: Multiply accumulator by memory[operand]
                acc *= Integer.parseInt(memory[operand], 2);
                pc++;
                break;
            case 10: // DIV: Divide accumulator by memory[operand], if not zero
                int divisor = Integer.parseInt(memory[operand], 2);
                if (divisor != 0) {
                    acc /= divisor;
                } else {
                    System.out.println("Error: Division by zero");
                }
                pc++;
                break;
            case 11: // AND: Logical AND between accumulator and memory[operand]
                acc = acc & Integer.parseInt(memory[operand], 2);
                pc++;
                break;
            case 12: // OR: Logical OR between accumulator and memory[operand]
                acc = acc | Integer.parseInt(memory[operand], 2);
                pc++;
                break;
            case 13: // XOR: Logical XOR between accumulator and memory[operand]
                acc = acc ^ Integer.parseInt(memory[operand], 2);
                pc++;
                break;
            case 14: // BRZ: Branch if accumulator is zero to memory[operand]
                if (acc == 0) {
                    pc = operand;
                } else {
                    pc++;
                }
                break;
            case 15: // STOP: Halt execution
                System.out.println("Execution halted.");
                System.exit(0);
                break;
            default:
                System.out.println("Unrecognized instruction: " + opcode);
                pc++;
                break;

        }
    }

    private void printState() {
        System.out.println("PC: " + pc + ", CI: " + ci + ", ACC: " + acc);
        System.out.println("Memory:");
        for (int i = 0; i < memorySize; i++) {
            System.out.println(i + ": " + memory[i]);
        }
    }
    public static void main(String[] args) {
        if (args.length < 2) {
            System.out.println("Usage: java BabySimulator <program file path> <memory size>");
            return;
        }
        try {
            int memorySize = Integer.parseInt(args[1]);
            BabySimulator simulator = new BabySimulator(args[0], memorySize);
            simulator.run();
        } catch (IOException e) {
            System.out.println("Failed to read program file.");
            e.printStackTrace();
        } catch (NumberFormatException e) {
            System.out.println("Invalid memory size.");
            e.printStackTrace();
        }
    }
}