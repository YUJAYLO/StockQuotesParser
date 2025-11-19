#ifndef CONVERTER_H
#define CONVERTER_H

#include <string>
#include <vector>
#include <cstdint>
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <windows.h>
#include <cmath>

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
        // oss << std::setw(intDigits + decDigits + 1) << std::setfill('0');
        oss << std::fixed << std::setprecision(decDigits) << PACKBCDToInt(buf, len)* pow(10, -decDigits);        

        std::string result = oss.str();
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

    inline std::string trim(const std::string& str) {
        const std::string whitespace = " \t\n\r\f\v";
        size_t first = str.find_first_not_of(whitespace);
        if (std::string::npos == first) {
            return "";
        }
        size_t last = str.find_last_not_of(whitespace);
        return str.substr(first, (last - first + 1));
    }

};

#endif // CONVERTER_H
