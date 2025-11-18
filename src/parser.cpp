#include "..\include\parser.h"
#include "..\include\format.h"
#include "..\include\utils\converter.h"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <cstring>

Parser::Parser(const std::string& filePath) : filePath(filePath) {}

void Parser::parseFile() {
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filePath);
    }

    std::vector<uint8_t> carry;
    std::vector<uint8_t> buffer(BLOCK_SIZE);
    while (file.read(reinterpret_cast<char*>(buffer.data()), BLOCK_SIZE) || file.gcount() > 0) {
        std::streamsize bytesRead = file.gcount();
        std::vector<uint8_t> block(buffer.begin(), buffer.begin() + bytesRead);

        processBlock(block, carry);
    }
    // after loop, if carry not empty, try final processing (maybe last record)
    if (!carry.empty()) {
        std::vector<uint8_t> empty;
        processBlock(empty, carry); // tries to flush final records
    }

    file.close();
}

void Parser::processBlock(const std::vector<uint8_t>& block, std::vector<uint8_t>& carry) {
    // concatenate carry + block
    std::vector<uint8_t> data;
    data.reserve(carry.size() + block.size());
    data.insert(data.end(), carry.begin(), carry.end());
    data.insert(data.end(), block.begin(), block.end());
    carry.clear();


    size_t i = 0;
    size_t totalSize = data.size();
    while (i < totalSize) {
        if (data[i] == Format::ESC_CODE) { // ESC-CODE
            if (i + 9 >= totalSize) {
                carry.assign(data.begin() + i, data.end());
                return;
            }

            size_t msgLen = static_cast<size_t>(Converter::PACKBCDToInt(&data[i + 1], 2)); // PACK BCD decoding
            if (i + msgLen > totalSize) {
                carry.assign(data.begin() + i, data.end());
                return;
            }

            std::vector<uint8_t> record(data.begin() + i, data.begin() + i + msgLen);
            if (validateRecord(record)) {
                handleRecord(record);
                i += msgLen;
            } else {
                i++; // move forward to find next ESC_CODE
            }
            
        } else {
            i++;
        }
    }
}

bool Parser::validateRecord(const std::vector<uint8_t>& record) {
    if (record.size() < 13) return false; // Minimum size check

    // terminal are last two bytes (positions endIdx-2, endIdx-1)
    uint16_t endBytes = (record[record.size() - 2] << 8) | record[record.size() - 1];
    if (endBytes != Format::TERMINAL_CODE) {
        return false;
    }

    uint8_t checksum = 0;
    for (size_t i = 1; i < record.size() - 3; i++) {
        checksum ^= record[i];
    }
    return checksum == record[record.size() - 3];
}

void Parser::handleRecord(const std::vector<uint8_t>& record) {
    const Format::HEADER* header = reinterpret_cast<const Format::HEADER*>(&record[1]);
    if (header->formatCode == 0x01) {
        Format::Format1 format1(record);
        format1.process();
    }
}