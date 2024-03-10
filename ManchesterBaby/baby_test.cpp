#include "Baby.h"
#include "Assembler.h"

int main() {

    Assembler assembler = Assembler();

    vector<bitset<32>> store = {
            bitset<32>("00000000000000000000000000000000"),
            bitset<32>("11100000000000100000000000000000"),
            bitset<32>("00010000000000010000000000000000"),
            bitset<32>("10010000000001100000000000000000"),
            bitset<32>("10010000000000100000000000000000"),
            bitset<32>("10010000000001100000000000000000"),
            bitset<32>("00000000000001110000000000000000"),
            bitset<32>("10000000001000000000000000000000"),
            bitset<32>("10110110010000000000000000000000"),
            bitset<32>("00000000000000000000000000000000")
    };


    for(int i = 10; i < 32; i++) {
        store.push_back(bitset<32>());
    }

    // Reverse the bits in each bitset
    for (int i = 0; i < 32; i++) {
        bitset<32> reversed;
        for (int j = 0; j < 32; j++) {
            reversed[j] = store[i][31 - j];
        }
        store[i] = reversed;
    }

    Baby baby = Baby(store);

    int x = 0;
    while (!baby.stopped) {
        baby.increment();

        cout << "\nStep: " << x << endl;
        x++;
        for (int i = 0; i < 32; i++) {

            cout << "Register: " << i << " " << reverse(baby.store[i]) << endl;
        }
    }
}
