#ifndef CONVERTER_H
#define CONVERTER_H

#include <string>
#include <vector>
#include <cstdint>
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <windows.h>

namespace Converter {

    inline int PACKBCDToInt(const uint8_t* buf, size_t len) {
        int val = 0;
        for (size_t i=0;i<len;i++){
            int hi = (buf[i] >> 4) & 0xF;
            int lo = (buf[i] & 0xF);
            val = val * 100 + hi*10 + lo;
        }

        return val;
    };

    inline std::string PACKBCDToString(const uint8_t* buf, size_t len, int intDigits, int decDigits) {
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

    inline std::string Big5ToUTF8(const std::string& big5) {
        if (big5.empty()) {
            return "";
        }

        // Get the required size for the wide char buffer
        int wideCharSize = MultiByteToWideChar(950, 0, big5.c_str(), -1, NULL, 0);
        if (wideCharSize == 0) {
            return big5; // Or throw an exception
        }

        // Allocate buffer for wide char string
        std::vector<wchar_t> wideCharBuffer(wideCharSize);
        MultiByteToWideChar(950, 0, big5.c_str(), -1, wideCharBuffer.data(), wideCharSize);

        // Get the required size for the UTF-8 buffer
        int utf8Size = WideCharToMultiByte(CP_UTF8, 0, wideCharBuffer.data(), -1, NULL, 0, NULL, NULL);
        if (utf8Size == 0) {
            return big5; // Or throw an exception
        }

        // Allocate buffer for UTF-8 string
        std::string utf8String;
        utf8String.resize(utf8Size - 1); // Resize to fit the null-terminated string, excluding the null terminator
        WideCharToMultiByte(CP_UTF8, 0, wideCharBuffer.data(), -1, &utf8String[0], utf8Size, NULL, NULL);

        return utf8String;
    }

};

#endif // CONVERTER_H
