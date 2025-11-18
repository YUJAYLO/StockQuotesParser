#ifndef CONVERTER_H
#define CONVERTER_H

#include <string>
#include <vector>
#include <cstdint>

namespace Converter {

    int PACKBCDToInt(const uint8_t* buf, size_t len);
    std::string PACKBCDToString(const uint8_t* buf, size_t len, int intDigits, int decDigits);
    std::string Big5ToUTF8(const std::string& big5);

};

#endif // CONVERTER_H