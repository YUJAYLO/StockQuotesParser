#include "..\include\format.h"
#include "..\include\dataAccess.h"
#include "..\include\utils\converter.h"
#include <iostream>
#include <cstring>
#include <ctime>

namespace Format {

Format1::Format1(const std::vector<uint8_t>& record) {
    const RawRecord<BODY_SIZE>* rawRecord = reinterpret_cast<const RawRecord<BODY_SIZE>*>(record.data());
    // std::memcpy(&rawRecord, record.data(), sizeof(RawRecord<BODY_SIZE>));
    
    if( rawRecord->header.messageLength != _messageLength ||
        rawRecord->header.businessType != _businessType ||
        rawRecord->header.version != _version) {
        std::cerr << "Warning: Unexpected HEADER values." << std::endl;
    }
    std::memcpy(&_body, rawRecord->body, sizeof(BODY));
}

void Format1::process() {

    ParsedRecord parsed;
    parsed.symbolCode = std::string(_body.symbolCode, 6);
    parsed.symbolShort = Converter::Big5ToUTF8(std::string(_body.symbolShort, 16));
    parsed.refPrice = Converter::PACKBCDToString(_body.refPrice, 5, 5, 4);
    parsed.upLimit = Converter::PACKBCDToString(_body.upLimit, 5, 5, 4);
    parsed.downLimit = Converter::PACKBCDToString(_body.downLimit, 5, 5, 4);

    // Get current timestamp
    std::time_t t = std::time(nullptr);
    char buf[20];
    std::strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%SZ", std::gmtime(&t));
    parsed.timestamp = buf;

    DataAccess::saveFormat1(parsed);
}

} // namespace Format