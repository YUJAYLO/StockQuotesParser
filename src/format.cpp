#include "..\include\format.h"
#include "..\include\dataAccess.h"
#include "..\include\converter.h"
#include <iostream>
#include <cstring>
#include <ctime>

namespace Format {

void Format1::process(const std::vector<uint8_t>& record) {
    const HEADER* header = reinterpret_cast<const HEADER*>(&record[1]);

    if (header->messageLength != 114 || header->businessType != 0x01 ||
        header->formatCode != 0x01 || header->version != 0x09) {
        std::cerr << "Warning: Unexpected HEADER values." << std::endl;
    }

    ParsedRecord parsed;
    parsed.symbolCode = std::string(reinterpret_cast<const char*>(&record[20]), 6);
    parsed.symbolShort = Converter::Big5ToUTF8(std::string(reinterpret_cast<const char*>(&record[10]), 10));
    parsed.refPrice = Converter::packBcdDecode(&record[26], 5, 5, 4);
    parsed.upLimit = Converter::packBcdDecode(&record[31], 5, 5, 4);
    parsed.downLimit = Converter::packBcdDecode(&record[36], 5, 5, 4);

    // Get current timestamp
    std::time_t t = std::time(nullptr);
    char buf[20];
    std::strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%SZ", std::gmtime(&t));
    parsed.timestamp = buf;

    DataAccess::saveFormat1(parsed);
}

} // namespace Format