#include "./include/parser.h"
#include <iostream>
#include <stdexcept>

int main() {
    try {
        Parser parser("./doc/modules.alias.bin");
        parser.parseFile();
        std::cout << "Parsing completed successfully." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}