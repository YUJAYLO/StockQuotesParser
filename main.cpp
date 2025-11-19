#include "./include/parser.h"
#include <iostream>

int main() {
    try {
        Parser parser("./doc/Tse.bin");
        parser.parseFile();
        std::cout << "Parsing completed successfully." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}