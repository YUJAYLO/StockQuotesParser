#include "..\include\format.h"
#include "..\include\dataAccess.h"
#include "..\include\utils\converter.h"
#include <iostream>
#include <cstring>
#include <ctime>
#include <stdexcept>

namespace Format {

Format1::Format1(const HEADER* header) {    
    uint16_t headerMessageLength = (header->messageLength[0] << 8) | header->messageLength[1];
    
    if( headerMessageLength != _messageLength ||
        header->businessType != _businessType ||
        header->version != _version) {
        throw std::runtime_error("Warning: Unexpected Format1 HEADER values.");
    }
}

void Format1::process(const std::vector<uint8_t>& rawBody) {
    const BODY* body = reinterpret_cast<const BODY*>(rawBody.data());

    ParsedRecord parsed;
    parsed.stockID = Converter::trim(std::string(body->stockID, 6));
    parsed.stockName = Converter::trim(Converter::Big5ToUTF8(std::string(body->stockName, 16)));
    parsed.refPrice = Converter::PACKBCDToString(body->refPrice, 5, 5, 4);
    parsed.upLimit = Converter::PACKBCDToString(body->upLimit, 5, 5, 4);
    parsed.downLimit = Converter::PACKBCDToString(body->downLimit, 5, 5, 4);

    DataAccess::saveFormat1(parsed);
}

} // namespace Format