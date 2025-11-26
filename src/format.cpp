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

Format6::Format6(const HEADER* header) {
    // Basic validation, can be expanded if we know exact version/length
    if (header->businessType != _businessType || header->version != _version) {
        throw std::runtime_error("Warning: Unexpected Format6 Business Type.");
    }
}

void Format6::process(const std::vector<uint8_t>& rawBody) {
    const BODY* body = reinterpret_cast<const BODY*>(rawBody.data());

    ParsedRecord parsed;
    parsed.stockID = Converter::trim(std::string(body->stockID, 6));
    parsed.matchingTime = Converter::PACKBCDToTimeStr(body->matchingTime, 6);

    size_t offset = 0;
    const uint8_t* variableData = body->realTimeQuotes;

    // Bit 7: Transaction Data
    if (body->revealItemNote & 0x80) {
        parsed.transactionPrice = Converter::PACKBCDToString(&variableData[offset], 5, 5, 4);
        offset += 5;
        parsed.transactionVol = Converter::PACKBCDToString(&variableData[offset], 4, 8, 0);
        offset += 4;
    }

    // Bits 6-4: Buy Quote Count
    int buyCount = (body->revealItemNote >> 4) & 0x07;
    for (int i = 0; i < buyCount; ++i) {
        std::string price = Converter::PACKBCDToString(&variableData[offset], 5, 5, 4);
        offset += 5;
        std::string vol = Converter::PACKBCDToString(&variableData[offset], 4, 8, 0);
        offset += 4;
        parsed.buyQuotes.push_back({price, vol});
    }

    // Bits 3-1: Sell Quote Count
    int sellCount = (body->revealItemNote >> 1) & 0x07;
    for (int i = 0; i < sellCount; ++i) {
        std::string price = Converter::PACKBCDToString(&variableData[offset], 5, 5, 4);
        offset += 5;
        std::string vol = Converter::PACKBCDToString(&variableData[offset], 4, 8, 0);
        offset += 4;
        parsed.sellQuotes.push_back({price, vol});
    }

    DataAccess::saveFormat6(parsed);
}

} // namespace Format