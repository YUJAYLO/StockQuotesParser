#include "converter.h"
#include <stdexcept>
#include <sstream>
#include <iomanip>

int Converter::PACKBCDToInt(const uint8_t* buf, size_t len) {
    int val = 0;
    for (size_t i=0;i<len;i++){
        int hi = (buf[i] >> 4) & 0xF;
        int lo = buf[i] & 0xF;
        val = val * 100 + hi*10 + lo;
    }

    return val;
};

std::string Converter::PACKBCDToString(const uint8_t* buf, size_t len, int intDigits, int decDigits) {
    std::ostringstream oss;
    for (size_t i = 0; i < len; ++i) {
        oss << std::hex << ((buf[i] >> 4) & 0xF) << (buf[i] & 0xF); // std::hex: 這是一個 I/O 操縱器 (manipulator)，它告訴 oss 後面的整數都應該以十六進位的格式來呈現。
    }
    std::string result = oss.str();
    if (result.size() != intDigits + decDigits) {
        throw std::runtime_error("Invalid PACK BCD length");
    }

    if(decDigits > 0)
        result.insert(result.end() - decDigits, '.');
    return result;
}

std::string Converter::Big5ToUTF8(const std::string& big5) {
    // Placeholder for Big5 to UTF-8 conversion
    return big5; // Return original if no conversion library is available
}