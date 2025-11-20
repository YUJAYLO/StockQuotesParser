#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include <cstdint>

class Parser {
public:
    explicit Parser(const std::string& filePath);
    void parseFile();

private:
    static constexpr size_t BLOCK_SIZE = 512 * 1024; // 512 KB
    std::string filePath;
    std::vector<uint8_t> leftoverBytes; // Store leftover bytes between blocks

    void processBlock(const std::vector<uint8_t>& block, std::vector<uint8_t>& carry, bool* escCodeFound);
    bool validateRecord(const std::vector<uint8_t>& record);
    void handleRecord(const std::vector<uint8_t>& record);
};

#endif // PARSER_H