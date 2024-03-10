#include "Baby.h"
#include "Assembler.h"
#include <iostream>
#include <bitset>
#include <vector>
#include <vector>

class Display {
public:
    Display() {}

    void displayStore(const std::vector<std::bitset<32>>& store) {
        std::string one = "\u2588"; // Character for 1: "█"
        std::string zero = "\u2591"; // Character for 0: "░"

        for (const auto& bitset : store) {
            for (int i = 0; i < 32; ++i) {
                std::cout << (bitset[i] ? one : zero);
            }
            std::cout << std::endl;
        }
    }
};
