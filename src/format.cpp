#include "..\include\format.h"
#include "..\include\dataAccess.h"
#include "..\include\utils\converter.h"
#include <iostream>
#include <cstring>
#include <ctime>
#include <stdexcept>

namespace Format {

Format1::Format1(const std::vector<uint8_t>& record) {
    const RawRecord<BODY_SIZE>* rawRecord = reinterpret_cast<const RawRecord<BODY_SIZE>*>(record.data());
    // std::memcpy(&rawRecord, record.data(), sizeof(RawRecord<BODY_SIZE>));
    
    uint16_t headerMessageLength = (rawRecord->header.messageLength[0] << 8) | rawRecord->header.messageLength[1];
    if( headerMessageLength != _messageLength ||
        rawRecord->header.businessType != _businessType ||
        rawRecord->header.version != _version) {
        throw std::runtime_error("Warning: Unexpected HEADER values.");
    }
    std::memcpy(&_body, rawRecord->body, sizeof(BODY));
}

void Format1::process() {

    ParsedRecord parsed;
    parsed.stockID = Converter::trim(std::string(_body.stockID, 6));
    parsed.stockName = Converter::trim(Converter::Big5ToUTF8(std::string(_body.stockName, 16)));
    parsed.refPrice = Converter::PACKBCDToString(_body.refPrice, 5, 5, 4);
    parsed.upLimit = Converter::PACKBCDToString(_body.upLimit, 5, 5, 4);
    parsed.downLimit = Converter::PACKBCDToString(_body.downLimit, 5, 5, 4);

    DataAccess::saveFormat1(parsed);
}

} // namespace Format